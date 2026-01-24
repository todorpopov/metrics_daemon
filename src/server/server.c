#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include "server.h"

int get_server_port() {
    char *port_str = getenv("SERVER_PORT");
    if (port_str != NULL) {
        return atoi(port_str);
    }
    return DEFAULT_PORT;
}

server_config_t* server_config_create(int port, request_handler handler) {
    server_config_t *config = malloc(sizeof(server_config_t));
    if (config == NULL) {
        perror("malloc");
        return NULL;
    }
    config->port = port;
    config->handler = handler;
    return config;
}

void server_config_free(server_config_t *config) {
    if (config != NULL) {
        free(config);
    }
}

static volatile sig_atomic_t g_stop_requested = 0;
static int g_listen_fd = -1;

static void handle_shutdown_signal(int signo) {
    (void)signo;
    g_stop_requested = 1;

    if (g_listen_fd >= 0) {
        close(g_listen_fd);
        g_listen_fd = -1;
    }
}

void server_install_signal_handlers(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_shutdown_signal;
    sigemptyset(&sa.sa_mask);

    sa.sa_flags = 0;

    if (sigaction(SIGTERM, &sa, NULL) < 0) {
        perror("sigaction(SIGTERM)");
    }
    if (sigaction(SIGINT, &sa, NULL) < 0) {
        perror("sigaction(SIGINT)");
    }
}

void server_run_until_complete(server_config_t *config) {
    if (config == NULL) {
        fprintf(stderr, "Server config is NULL\n");
        return;
    }
    
    int listen_fd;
    int connfd;
    socklen_t client_len;
    struct sockaddr_in server_addr, client_addr;
    char buffer[MAXLINE];

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("socket");
        return;
    }

    g_listen_fd = listen_fd;

    int opt = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(listen_fd);
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(config->port);
    
    if (bind(listen_fd, (SA *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(listen_fd);
        g_listen_fd = -1;
        return;
    }

    if (listen(listen_fd, LISTEN_QUEUE_SIZE) < 0) {
        perror("listen");
        close(listen_fd);
        g_listen_fd = -1;
        return;
    }
    
    printf("Server listening on port %d\n", config->port);

    while (!g_stop_requested) {
        client_len = sizeof(client_addr);
        connfd = accept(listen_fd, (SA *)&client_addr, &client_len);
        
        if (connfd < 0) {
            if (g_stop_requested) {
                break;
            }

            if (errno == EINTR) {
                continue;
            }

            perror("accept");
            continue;
        }

        memset(buffer, 0, sizeof(buffer));
        ssize_t n = recv(connfd, buffer, MAXLINE - 1, 0);
        
        if (n > 0) {
            config->handler(connfd, buffer);
        }
        
        close(connfd);
    }

    if (g_listen_fd >= 0) {
        close(g_listen_fd);
        g_listen_fd = -1;
    }

    printf("Server stopped\n");
}

void send_error_response(int connfd, const char *error_message) {
    char response[256];

    if (error_message == NULL) {
        error_message = "Internal Server Error";
    }

    size_t msg_len = strlen(error_message);
    if (msg_len >= sizeof(response)) {
        error_message = "Internal Server Error";
    }

    snprintf(
        response,
        sizeof(response),
        "HTTP/1.1 500 Internal Server Error\r\nContent-Length: %lu\r\n\r\n%s",
        (unsigned long)strlen(error_message),
        error_message
    );
    send(connfd, response, strlen(response), 0);
}

void send_json_response(int connfd, const char *json_body) {
    char response[2048];

    if (json_body == NULL) {
        json_body = "{}";
    }

    size_t body_len = strlen(json_body);

    int header_len = snprintf(
        response,
        sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %lu\r\n"
        "Connection: close\r\n"
        "\r\n",
        (unsigned long)body_len
    );

    if (header_len < 0) {
        return;
    }

    size_t header_sz = (size_t)header_len;
    if (header_sz + body_len < sizeof(response)) {
        memcpy(response + header_sz, json_body, body_len);
        send(connfd, response, header_sz + body_len, 0);
    } else {
        send(connfd, response, header_sz, 0);
        send(connfd, json_body, body_len, 0);
    }
}

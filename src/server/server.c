#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
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
        return;
    }

    if (listen(listen_fd, LISTEN_QUEUE_SIZE) < 0) {
        perror("listen");
        close(listen_fd);
        return;
    }
    
    printf("Server listening on port %d\n", config->port);

    while (1) {
        client_len = sizeof(client_addr);
        connfd = accept(listen_fd, (SA *)&client_addr, &client_len);
        
        if (connfd < 0) {
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
    
    close(listen_fd);
}

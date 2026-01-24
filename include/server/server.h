#ifndef SERVER_H
#define SERVER_H

#define DEFAULT_PORT 8080
#define MAXLINE 1024
#define SA struct sockaddr
#define LISTEN_QUEUE_SIZE 10


typedef void (*request_handler)(int connfd, const char *request);

typedef struct {
    int port;
    request_handler handler;
} server_config_t;

int get_server_port();
server_config_t* server_config_create(int port, request_handler handler);
void server_run_until_complete(server_config_t *config);
void server_config_free(server_config_t *config);

void send_error_response(int connfd, const char *error_message);

void server_install_signal_handlers(void);
void send_json_response(int connfd, const char *json_body);

#endif
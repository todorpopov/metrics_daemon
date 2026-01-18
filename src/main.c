#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include "metrics.h"
#include "server.h"

void handle_request(int connfd, const char *request __attribute__((unused))) {
    const char *body = read_mem_file(MEM_FILE_PATH);
    if (body == NULL) {
        const char *error_response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 21\r\n\r\nInternal Server Error";
        send(connfd, error_response, strlen(error_response), 0);
        return;
    }

    char response[8192];
    int body_len = strlen(body);
    snprintf(response, sizeof(response),
             "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %d\r\n\r\n%s",
             body_len, body);
    send(connfd, response, strlen(response), 0);
}

int main() {
    server_config_t *config = server_config_create(8080, handle_request);
    server_run_until_complete(config);
    server_config_free(config);
    return 0;
}

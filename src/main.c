#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "metrics.h"
#include "server.h"

void handle_request(int connfd, const char *request __attribute__((unused))) {
    enum metrics_format format = FORMAT_JSON;

    const char *body = get_metrics(format);
    if (body == NULL) {
        const char *error_response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 21\r\n\r\nInternal Server Error";
        send(connfd, error_response, strlen(error_response), 0);
        return;
    }

    char response[8192];
    int body_len = strlen(body);
    switch (format) {
        case FORMAT_JSON:
            snprintf(
                response,
                sizeof(response),
                "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %d\r\n\r\n%s",
                body_len,
                body
            );
            break;
        default:
            body = "Unsupported format";
            snprintf(
                response,
                sizeof(response),
                "HTTP/1.1 400 Bad Request\r\nContent-Length: %d\r\n\r\n%s",
                (int)strlen(body),
                body
            );
            break;
    }

    send(connfd, response, strlen(response), 0);
}

int main() {
    server_config_t *config = server_config_create(8080, handle_request);
    server_run_until_complete(config);
    server_config_free(config);
    return 0;
}

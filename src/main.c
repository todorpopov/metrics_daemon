#include <sys/socket.h>
#include <string.h>

#include "metrics.h"
#include "server.h"

void handle_request_json(int connfd, const char *request __attribute__((unused))) {
    struct metrics_t metrics = {0};
    const int success = get_metrics(&metrics);
    if (success != 0) {
        send_error_response(connfd, "Failed to get metrics");
        return;
    }

    const char *metrics_json = format_metrics_json(&metrics);
    if (metrics_json == NULL) {
        send_error_response(connfd, "Failed to format metrics");
        return;
    }
    send_json_response(connfd, metrics_json);
}

int main() {
    server_config_t *config = server_config_create(8080, handle_request_json);
    server_run_until_complete(config);
    server_config_free(config);
    return 0;
}

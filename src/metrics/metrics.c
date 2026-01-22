#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "metrics.h"
#include "memory.h"
#include "host.h"

char *get_metrics(enum metrics_format format) {
    char *mem_metrics = get_memory_metrics(format);
    char *host_metrics = get_host_name(format);

    if (mem_metrics == NULL || host_metrics == NULL) {
        return NULL;
    }

    char *result = NULL;

    switch (format) {
    case FORMAT_JSON:
        asprintf(&result, "{ \"memory\": %s, \"host\": %s }", mem_metrics, host_metrics);
        break;
    default:
        return NULL;
    }

    return result;
}

char *format_response(enum metrics_format format, const char *body) {
    static char response[8192];
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
                body_len,
                body
            );
            break;
    }
    return response;
}

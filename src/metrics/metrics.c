#define GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "metrics.h"
#include "memory.h"
#include "host.h"

int get_metrics(struct metrics_t *metrics) {
    const char *raw_mem_info = read_mem_file(get_mem_file_path());
    const char *host_name = read_host_name(get_host_file_path());
    if (raw_mem_info == NULL || host_name == NULL) {
        return 1;
    }

    metrics->mem_info.total_memory = 0;
    metrics->mem_info.free_memory = 0;
    parse_mem_info(&metrics->mem_info, raw_mem_info);

    strncpy(metrics->host_name, host_name, sizeof(metrics->host_name) - 1);
    metrics->host_name[sizeof(metrics->host_name) - 1] = '\0';

    return 0;
}

char *format_metrics_json(const struct metrics_t *metrics) {
    static char json[512];
    snprintf(
        json,
        sizeof(json),
        "{ \"memory\": { \"total_memory\": %lu, \"free_memory\": %lu }, \"host_name\": \"%s\" }",
        metrics->mem_info.total_memory,
        metrics->mem_info.free_memory,
        metrics->host_name
    );
    return json;
}

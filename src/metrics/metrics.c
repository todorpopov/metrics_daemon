#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>

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
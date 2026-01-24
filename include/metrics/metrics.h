#ifndef METRICS_H
#define METRICS_H

#include "memory.h"

enum metrics_format {
    FORMAT_JSON = 1,
};

struct metrics_t {
    struct mem_info_t mem_info;
    char host_name[256];
};

int get_metrics(struct metrics_t *metrics);
char *format_metrics_json(const struct metrics_t *metrics);

#endif
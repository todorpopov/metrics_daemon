#ifndef MEMORY_H
#define MEMORY_H

#include "metrics.h"

#define MEM_FILE_PATH "/proc/meminfo"

struct mem_info {
    unsigned long total_memory;
    unsigned long free_memory;
};

char *read_mem_file(const char *mem_file_path);
void parse_mem_info(const char *mem_file_content, struct mem_info *info);
char *format_mem_info(const struct mem_info *info, enum metrics_format format);
char *get_memory_metrics(enum metrics_format format);

#endif
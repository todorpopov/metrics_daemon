#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "metrics.h"
#include "memory.h"

#define MAX_LINE_LENGTH 256
#define MAX_LINES 256

char *get_mem_file_path() {
    char *mem_file_path_str = getenv("MEM_FILE_PATH_OVERRIDE");
    if (mem_file_path_str != NULL) {
        return mem_file_path_str;
    }
    return MEM_FILE_PATH;
}

char *read_mem_file(const char *mem_file_path) {
    printf("Starting read_mem_file\n");
    printf("Opening file: %s\n", mem_file_path);

    FILE *file = fopen(mem_file_path, "r");
    static char buffer[MAX_LINE_LENGTH * MAX_LINES] = "";
    buffer[0] = '\0';

    if (file == NULL) {
        fprintf(stderr, "Failed to open path: %s\n", mem_file_path);
        return NULL;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strlen(buffer) + strlen(line) <= sizeof(buffer)) {
            strcat(buffer, line);
        }
    }

    fclose(file);
    return buffer;
}

void parse_mem_info(const char *mem_file_content, struct mem_info *info) {
    printf("Starting parse_mem_info\n");

    const char *line = mem_file_content;
    char key[32];
    unsigned long value;
    char unit[16];

    while (*line) {
        if (sscanf(line, "%31s %lu %15s", key, &value, unit) == 3) {
            if (strcmp(key, "MemTotal:") == 0) {
                info->total_memory = value;
                printf("Parsed MemTotal: %lu %s\n", value, unit);
            } else if (strcmp(key, "MemFree:") == 0) {
                info->free_memory = value;
                printf("Parsed MemFree: %lu %s\n", value, unit);
            }
        }

        while (*line && *line != '\n') {
            line++;
        }
        if (*line == '\n') {
            line++;
        }
    }
}

char *format_mem_info(const struct mem_info *info, enum metrics_format format) {
    printf("Starting format_mem_info\n");
    static char formatted[512];

    switch (format)
    {
        case FORMAT_JSON:
            snprintf(
                formatted,
                sizeof(formatted),
                "{ \"total_memory_kb\": %lu, \"free_memory_kb\": %lu }",
                info->total_memory,
                info->free_memory);
            break;
        default:
            snprintf(formatted, sizeof(formatted), "Unsupported format");
            break;
    }

    return formatted;
}

char *get_memory_metrics(enum metrics_format format) {
    printf("Starting get_memory_metrics\n");

    char *mem_file_content = read_mem_file(get_mem_file_path());
    if (mem_file_content == NULL) {
        return NULL;
    }

    struct mem_info info = {0};
    parse_mem_info(mem_file_content, &info);

    char *formatted_info = format_mem_info(&info, format);
    return formatted_info;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void parse_mem_info(struct mem_info_t *info, const char *mem_file_content) {
    printf("Starting parse_mem_info\n");
    if (mem_file_content == NULL) {
        return;
    }

    const char *line = mem_file_content;
    char key[32];
    unsigned long value;

    while (*line) {
        char unit[16];
        if (sscanf(line, "%31s %lu %15s", key, &value, unit) == 3) {
            if (strcmp(key, "MemTotal:") == 0) {
                info->total_memory = value;
            } else if (strcmp(key, "MemFree:") == 0) {
                info->free_memory = value;
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

char *format_mem_info_json(const struct mem_info_t *info) {
    static char json[256];
    snprintf(
        json,
        sizeof(json),
        "{ \"total_memory_kb\": %lu, \"free_memory_kb\": %lu }",
        info->total_memory,
        info->free_memory);

    return json;
}

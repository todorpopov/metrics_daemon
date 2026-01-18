#include <stdio.h>
#include <string.h>
#include "metrics.h"

#define MAX_LINE_LENGTH 256
#define MAX_LINES 256

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

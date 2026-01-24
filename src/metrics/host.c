#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "host.h"

char *get_host_file_path() {
    char *host_file_path_str = getenv("HOST_FILE_PATH_OVERRIDE");
    if (host_file_path_str != NULL) {
        return host_file_path_str;
    }
    return HOST_FILE_PATH;
}

char *read_host_name(const char *host_file_path) {
    printf("Opening file: %s\n", host_file_path);

    FILE *file = fopen(host_file_path, "r");
    static char hostname[256] = "";
    hostname[0] = '\0';

    if (file == NULL) {
        fprintf(stderr, "Failed to open path: %s\n", host_file_path);
        return NULL;
    }

    if (fgets(hostname, sizeof(hostname), file) != NULL) {
        size_t len = strlen(hostname);
        if (len > 0 && hostname[len - 1] == '\n') {
            hostname[len - 1] = '\0';
        }
    }

    fclose(file);
    printf("Host name: %s\n", hostname);
    return hostname;
}

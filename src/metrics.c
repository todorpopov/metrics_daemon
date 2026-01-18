#include <stdio.h>
#include "metrics.h"

int get_mem_free() {
    return 1;
}

int get_mem_total() {
    return 2;
}

int get_num_cpu() {
    return 3;
}

void get_metrics(struct metrics_t *m) {
    m->mem_free = get_mem_free();
    m->mem_total = get_mem_total();
    m->num_cpu = get_num_cpu();
}

void print_metrics(struct metrics_t *m) {
    printf("MemoryFree: %d\n", m->mem_free);
    printf("MemoryTotal: %d\n", m->mem_total);
    printf("NumCPU: %d\n", m->num_cpu);
}

void read_mem_file() {
    FILE *file = fopen("/proc/meminfo", "r");
    if (file == NULL) {
        perror("Failed to open /proc/meminfo");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

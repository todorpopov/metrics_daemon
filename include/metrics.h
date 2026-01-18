#ifndef METRICS_H
#define METRICS_H

struct metrics_t {
    int mem_free, mem_total, num_cpu;
};

int get_mem_free();
int get_mem_total();
int get_num_cpu();

void get_metrics(struct metrics_t *m);
void print_metrics(struct metrics_t *m);

void read_mem_file();

#endif
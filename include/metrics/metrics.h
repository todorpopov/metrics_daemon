#ifndef METRICS_H
#define METRICS_H

#define MEM_FILE_PATH "/proc/meminfo"

char *read_mem_file(const char *mem_file_path);

#endif
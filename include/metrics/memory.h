#ifndef MEMORY_H
#define MEMORY_H

#define MEM_FILE_PATH "/proc/meminfo"

char *get_mem_file_path();

struct mem_info_t {
    unsigned long total_memory;
    unsigned long free_memory;
};

char *read_mem_file(const char *mem_file_path);
void parse_mem_info(struct mem_info_t *info, const char *mem_file_content);
char *format_mem_info_json(const struct mem_info_t *info);

#endif
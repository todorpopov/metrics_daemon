#ifndef HOST_H
#define HOST_H

#define HOST_FILE_PATH "/proc/sys/kernel/hostname"

char *read_host_name(const char *host_file_path);
char *format_host_name(const char *host_name, enum metrics_format format);
char *get_host_name(enum metrics_format format);

#endif
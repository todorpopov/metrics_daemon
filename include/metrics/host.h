#ifndef HOST_H
#define HOST_H

#define HOST_FILE_PATH "/proc/sys/kernel/hostname"

char *get_host_file_path();
char *read_host_name(const char *host_file_path);

#endif
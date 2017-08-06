#ifndef MMAP_UTILS_H
#define MMAP_UTILS_H

#include <unistd.h>
#include <stdint.h>

typedef struct {
    const size_t sz;
    uint8_t *dat;
    const size_t sz_aligned;
    const char *file_name;
    const int fd;
} mapped_file_t;

typedef enum {
    READ,
    READWRITE
} open_mode_t;

mapped_file_t map_file_create(const char *file_name,
                              const size_t size);

mapped_file_t map_file_open(const char *file_name,
                            const open_mode_t open_mode);

void map_file_close(const mapped_file_t mf);

#endif

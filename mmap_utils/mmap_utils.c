#include "mmap_utils.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

mapped_file_t map_file_create(const char *file_name,
                              const size_t size) {

    int fd = open(file_name, O_RDWR | O_CREAT, (mode_t)0644);
    ftruncate(fd, size);
    int64_t pagesize = sysconf(_SC_PAGE_SIZE);
    int64_t sz_aligned = size + pagesize - (size % pagesize);
    uint8_t *mmapPtr = mmap(0, sz_aligned, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    mapped_file_t mf = {
        .sz = size,
        .dat = mmapPtr,
        .sz_aligned = sz_aligned,
        .file_name = file_name,
        .fd = fd
    };

    return mf;
}

mapped_file_t map_file_open(const char *file_name,
                            const open_mode_t open_mode) {

    int file_flags, map_flags;
    switch(open_mode) {
        case READWRITE:
            file_flags = O_RDWR;
            map_flags = PROT_READ | PROT_WRITE;
            break; 
        default:
            file_flags = O_RDONLY;
            map_flags = PROT_READ;
            break;
    }

    struct stat sbuf;
    int fd = open(file_name, file_flags);
    fstat(fd, &sbuf);
    size_t szBytes = sbuf.st_size;
    int64_t pagesize = sysconf(_SC_PAGE_SIZE);
    int64_t sz_aligned = szBytes + pagesize - (szBytes % pagesize);
    uint8_t *mmapPtr = mmap(0, sz_aligned, map_flags, MAP_SHARED, fd, 0);

    mapped_file_t mf = {
        .sz = szBytes,
        .dat = mmapPtr,
        .sz_aligned = sz_aligned,
        .file_name = file_name,
        .fd = fd
    };

    return mf;
}

void map_file_close(const mapped_file_t mf) {
  munmap(mf.dat, mf.sz_aligned);
  close(mf.fd);
}


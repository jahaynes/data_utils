#include "mmap_utils.h"

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


int main(int argc, char** argv) {
    mapped_file_t mf = map_file_open(argv[1], READ); 
    mf.dat[1] = 65;
    map_file_close(mf);
    return 0;
}

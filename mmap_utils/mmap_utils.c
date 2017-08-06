#include "mmap_utils.h"

mapped_file_t map_file_open(const char *file_name) {

    struct stat sbuf;
    int fd = open(file_name,O_RDONLY);
    fstat(fd, &sbuf);
    size_t szBytes = sbuf.st_size;
    int64_t pagesize = sysconf(_SC_PAGE_SIZE);
    int64_t sz_aligned = szBytes + pagesize - (szBytes % pagesize);
    uint8_t *mmapPtr = mmap(0, sz_aligned, PROT_READ, MAP_SHARED, fd, 0);

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
    return 0;
}

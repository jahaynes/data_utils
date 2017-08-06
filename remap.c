#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define FILEPATH "./myfile"
#define NUMINTS  (4)
#define FILESIZE (NUMINTS * sizeof(int))

int main(int argc, char *argv[]) {
    
    int *map;  /* mmapped array of ints */

    int fd = open(FILEPATH, O_RDWR | O_CREAT, (mode_t)0600);

    ftruncate(fd, 16);
    
    //lseek(fd, FILESIZE-1, SEEK_SET);
    //write(fd, "", 1);

    map = mmap(0, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    //map[4] = 65;

    munmap(map, FILESIZE);

    close(fd);
    return 0;
}

#include <stdio.h>
#include "mk_linux_programming_interface/tlpi-dist/lib/tlpi_hdr.h"
#include <fcntl.h>


int main(int argc, char* argv[]) {
    int fd = atoi(argv[1]);
    int flags = O_RDWR | O_CREAT | O_APPEND;
    fd = open("test", flags); 

    int dupfd = dup(fd);

    int original_flags = fcntl(fd, F_GETFL);
    int dupflags = fcntl(dupfd, F_GETFL);

    write(fd, "aaa", 3);

    off_t originaloffset = lseek(fd, 0, SEEK_CUR);
    off_t dupoffset = lseek(dupfd, 0, SEEK_CUR);

    printf("flags, offset:%x %d:%x:%d %d:%x:%d", flags, fd, original_flags, originaloffset, dupfd, dupflags, dupoffset);

    return EXIT_SUCCESS;
}

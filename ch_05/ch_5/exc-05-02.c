#include <stdio.h>
#include <fcntl.h>
#include "mk_linux_programming_interface/tlpi-dist/lib/tlpi_hdr.h"

int main (int argc, char* argv[])
{
    int fd;
    off_t offset;
    if (argc != 2)
    {
        printf("Usage: %s filename\n", argv[0]);
        exit(1);
    }

    
    fd = open(argv[1], O_WRONLY | O_APPEND);
    if (-1 == fd)
    {
        printf("Error at open\n");
        exit(2);
    }

    //lseek(fd, 0, SEEK_END);
    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        printf("Error at lseek\n");
        exit(3);
    }
    int write_result = write(fd, "test\n", 5);
    if (-1 == write_result)
    {
        printf("Error in write\n");
        exit(4);
    }

    printf("write success! write_result=%d\n", write_result);

    close(fd);

    return EXIT_SUCCESS;
}

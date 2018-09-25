#include <stdio.h>
#include <fcntl.h>
#include </home/robert/Workspace/Linux/mk_linux_programming_interface/tlpi-dist/lib/tlpi_hdr.h>

int main(int argc, char* argv[])
{
    if (argc < 3 || argc > 5)
    {
        printf("Usage error\n");
        exit(1);
    }

    int flags = O_RDWR | O_CREAT;
    if (argc == 3)
    {
        flags |= O_APPEND;
    }

    int fd = open(argv[1], flags, S_IRUSR | S_IWUSR);
    if (-1 == fd)
    {
        printf("Opening error!\n");
        exit(2);
    }
    
    int num_bytes = atoi(argv[2]);

    for (int i = 0; i < num_bytes; ++i)
    {
        if (argc == 4)
        {
            lseek(fd, 0, SEEK_END);
        }
        write(fd, "a", 1);
    } 
    
    printf("Writing successfull.\n");

    return EXIT_SUCCESS;
}

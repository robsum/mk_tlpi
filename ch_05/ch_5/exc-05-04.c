#include <stdio.h>
#include <fcntl.h>
#include <mk_linux_programming_interface/tlpi-dist/lib/tlpi_hdr.h>

extern errno;

int my_dup(int oldfd);
int my_dup2(int oldfd, int newfd);

int main(int argc, char* argv[])
{
    int oldfd = atoi(argv[1]);
    int newfd = my_dup(oldfd);
    
    printf("my_dup newfd: %d\n", newfd);
    write(newfd, argv[2], strlen(argv[2]));
    
    int newfd2 = my_dup2(oldfd, 10);
    printf("my_dup2 newfd: %d\n", newfd2);
    write(newfd2, argv[2], strlen(argv[2]));

    return EXIT_SUCCESS;
}

int my_dup(int oldfd)
{
    int flags = fcntl(oldfd, F_GETFL);
    if (flags == -1) { errExit("fcntl"); }
    return fcntl(oldfd, F_DUPFD);

}

int my_dup2(int oldfd, int newfd)
{
    if (oldfd == newfd) {
        int valid = fcntl(oldfd, F_GETFL);
        if (!valid) {
            errno = EBADF;
            return -1;
        }
        return oldfd;
    }
    
    int exist = fcntl(newfd, F_GETFL);
    if (exist != -1) { close(newfd); }

    return fcntl(oldfd, F_DUPFD, newfd);
}

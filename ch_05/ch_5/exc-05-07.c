#include <stdio.h>
#include <fcntl.h>
#include <mk_linux_programming_interface/tlpi-dist/lib/tlpi_hdr.h>
#include <sys/stat.h>
#include <sys/uio.h>

ssize_t my_readv(int fd, const struct iovec *iov, int iovcnt);
ssize_t my_writev(int fd, const struct iovec *iov, int iovcnt);

int main(int argc, char* argv[]) {
    struct iovec iov[3];
    struct stat myStruct;
    int x;
    char str[100];

    int totRequired = 0;
    
    iov[0].iov_base = &myStruct;
    iov[0].iov_len = sizeof(struct stat);
    totRequired += iov[0].iov_len;
    iov[1].iov_base = &x;
    iov[1].iov_len = sizeof(x);
    totRequired += iov[1].iov_len;
    iov[2].iov_base = str;
    iov[2].iov_len = 100;
    totRequired += iov[2].iov_len;
    
    int fd = open(argv[1], O_RDWR | O_APPEND);
    ssize_t numRead = 0;//readv(fd, iov, 3);
    ssize_t numReadMy = my_readv(fd, iov, 3);

    printf("%d:%d\n", numRead, numReadMy);

    ssize_t numWritten = writev(fd, iov, 3);
    ssize_t numWrittenMy = my_writev(fd, iov, 3);
    printf("%d:%d\n", numWritten, numWrittenMy);

    return EXIT_SUCCESS;
}

ssize_t my_readv(int fd, const struct iovec *iov, int iovcnt) {
    int noOfTotalRead = 0;
    for (int i = 0; i < iovcnt; ++i) {
        noOfTotalRead += read(fd, iov[i].iov_base, iov[i].iov_len);
    }
    return noOfTotalRead;
}

ssize_t my_writev(int fd, const struct iovec *iov, int iovcnt) {
    #define size = 1000;
    int noOfTotalWritten = 0;
    for (int i = 0; i < iovcnt; ++i) {
        noOfTotalWritten += write(fd, iov[i].iov_base, iov[i].iov_len);
    }
    return noOfTotalWritten;
    
}

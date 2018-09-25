#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdio.h>
#include "mk_linux_programming_interface/tlpi-dist/lib/tlpi_hdr.h"

int main (int argc, char* argv[])
{
    int opt;
    Boolean aopt = FALSE;
    opt = getopt(argc, argv, "a");
    switch (opt)
    {
        case 'a': aopt = TRUE; break;
        default: break;
    }

    char* outputFile = argv[optind];

    //printf("aopt = %s\n", aopt == TRUE ? "TRUE" : "FALSE");
    //printf("outputFile = %s\n", outputFile);
    
    int openFlags = O_WRONLY | O_CREAT;
    if (TRUE == aopt)
    {
        openFlags |= O_APPEND;
    }
     
    int fd = open(outputFile, openFlags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    if (fd == -1) printf("error: open");


    int buflen = 64;
    int numRead;

    char* buf = malloc(buflen);

    while ( ( numRead = read(STDIN_FILENO, buf, buflen) ) > 0 )
    {
        write(fd, buf, numRead);
        write(STDOUT_FILENO, buf, numRead);
    }

    free(buf);


    return EXIT_SUCCESS;
}

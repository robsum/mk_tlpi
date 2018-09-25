#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdio.h>
#include "mk_linux_programming_interface/tlpi-dist/lib/tlpi_hdr.h"

int main (int argc, char* argv[])
{
    if (argc < 1)
       printf("argc < 1");
    char* inputFile  = argv[1];
    char* outputFile = argv[2];

    //printf("aopt = %s\n", aopt == TRUE ? "TRUE" : "FALSE");
    
    int openFlags = O_WRONLY | O_CREAT;
     
    int fdInput = open(inputFile, O_RDONLY);
    int fdOutput = open(outputFile, openFlags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    if (fdInput == -1 || fdOutput == -1)
        printf("error: open");


    int buflen = 1;
    int numRead;

    char* buf = malloc(buflen);

    while ( ( numRead = read(fdInput, buf, buflen) ) > 0 )
    {
        if ('\0' == buf[0])
        {
            // hole started
            off_t offset = 1;//lseek(fdInput, 0, SEEK_DATA);
            lseek(fdOutput, offset, SEEK_CUR);
            printf("h");
        }
        else
        {
            write(fdOutput, buf, numRead);
        }
    }

    free(buf);


    return EXIT_SUCCESS;
}

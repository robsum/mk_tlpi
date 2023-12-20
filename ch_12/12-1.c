#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include "mk_linux_programming_interface/tlpi-dist/lib/ugid_functions.h"

int main(int argc, char* argv[])
{

    if (argc < 2)
    {
        printf("usage: %s username\n", argv[0]);
        exit(1);
    }

    const char* username = argv[1];
    
    int userid = userIdFromName(username);

    const char* procdirname = "/proc/";
    DIR* procdirstream = opendir(procdirname);

    errno = 0;
    struct dirent* procdir;
    char fullstatusfilename[2048];
    char line[1024];
    //char namelinetolook[100];
    //char uidlinetolook[100];
    char statusname[100];
    printf("List of processes owned by user %s (uid: %d):\n", username, userid);
    while ((procdir = readdir(procdirstream)) != NULL && errno == 0)
    {
        //printf("next iteration: %s directory.\n", procdir->d_name);
        //if (procdir->d_type != DT_DIR)
        //{
        //    printf("continue\n");
        //    continue;
        //}
        if (strcmp(procdir->d_name, ".") == 0 || strcmp(procdir->d_name, "..") == 0)
        {
            continue;
        }
        //printf("%s d_type = %d\n", procdir->d_name, procdir->d_type, procdir->d_type);
        strcpy(fullstatusfilename, procdirname);
        strcat(fullstatusfilename, procdir->d_name);
        strcat(fullstatusfilename, "/status");
        FILE* fp = fopen(fullstatusfilename, "r");
        if (fp == NULL)
        {
            //printf("File %s does not exist. Exiting.\n", fullstatusfilename);
            //exit(EXIT_FAILURE);
            //printf("File %s does not exist. Continue...\n", fullstatusfilename);
            errno = 0;
            continue;
        }
        ssize_t nread;
        size_t len = 0;
        char* line = NULL;
        errno = 0;
        while ((nread = getline(&line, &len, fp)) != -1)
        {
            //printf("processing line \"%s\"\n", line);
            if (errno != 0)
            {
                printf("getline failure: errno=%d. Exiting.\n", errno);
                exit(EXIT_FAILURE);
            }
            if (strncmp(line, "Name", 4) == 0)
            {
                //calculate whitecharacters:
                int i = 5;
                while (isblank(line[i]))
                {
                   ++i;
                }
                strncpy(statusname, line+i, strlen(line+i) - 1);
                statusname[strlen(line+i)] = '\0';
                //printf("Checking if name %s == %s\n", statusname, username);
                //if (strcmp(statusname, username) == 0)
                //{
                //    printf("Process %s is owned by user %s\n", procdir->d_name, statusname);
                //}
            }
            if (strncmp(line, "Uid", 3) == 0)
            {
                int statusuid = atoi(line+4);
                //printf("Checking if uid %d == %d\n", statusuid, userid);
                if (statusuid == userid)
                {
                    printf("Process %s (%s) is owned by user id %d\n", procdir->d_name, statusname, statusuid);
                }

            }

            free(line);
            line = NULL;
            errno = 0;
        }
        if (fp != NULL)
        {
            fclose(fp);
        }
    }

    return EXIT_SUCCESS;
}

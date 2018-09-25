#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buf[4096];
extern char** environ;

enum bool {false = 0, true};

int setenv(const char *name, const char *value, int overwrite)
{
    // using getenv, putenv, directly modifying environ
    if (false == overwrite)
    {
        //
        if (NULL != getenv(name))
        {
            // env variable exists, exit
            printf("Variable %s already exists with a value %s!\n", name, getenv(name));
            exit(3);
        }
    }
    strcpy(buf, name);
    strcat(buf, "=");
    strcat(buf, value);

    return putenv(buf);
}

int unsetenv(const char *name)
{
    // using getenv, putenv, directly modifying environ
    // check if there are multiple difinitions of an env variable and remove them all
    while (getenv(name) != NULL)
    {
        int index = 0;

        while (strncmp(environ[index], name, strlen(name) != 0))
        {
            // line doesn't start with name, get to next line
            ++index;
        }
        // here we have the correct index,
        while (NULL != environ[index+1]) // next environ[x] is not null
        {
            //
            strcpy(environ[index], environ[index+1]);
            ++index;
        }
        environ[index] = NULL;
    }
}

int main(int argc, char* argv[])
{
    //
    if (argc < 2)
    {
        printf("Not enough function args.\n");
        exit(1);
    }

    int set = false;
    
    if (strcmp("set",argv[1]) == 0)
        set = true;
    else if (strcmp("unset", argv[1]) == 0)
        set = false;
    else
    {
        printf("Nor set or unset.\n");
        exit(2);
    }

    if (set)
    {
        setenv(argv[2], argv[3], true);
        printf("Variable %s set to value %s. Getenv=%s\n", argv[2], argv[3], getenv(argv[2]));
    }
    else
    {
        unsetenv(argv[2]);
        printf("Variable %s unset. Getenv=%s\n", argv[2], getenv(argv[2]));
    }

    return EXIT_SUCCESS;
}

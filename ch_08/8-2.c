#include <stdio.h>
#include <pwd.h>
#include <string.h>
#include "tlpi_hdr.h"

struct passwd* getpwnam_my(const char *name)
{
    // using setpwent(), getpwent(), endpwent()
    static struct passwd *pwd_s;

    while ((pwd_s = getpwent()) != NULL)
    {
        if (strcmp(name, pwd_s->pw_name) == 0)
        {
            endpwent();
            return pwd_s;
        }
    }
    endpwent();
    return NULL;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Usage: %s username\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    struct passwd* pwd_s = getpwnam_my(argv[1]);
    printf("User %s has %s home directory.\n", argv[0], pwd_s == NULL ? "no such user" : pwd_s->pw_dir);

    return EXIT_SUCCESS;
}

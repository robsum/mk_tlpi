#include <stdio.h>
#include <string.h>
#include <grp.h> // setgroups()
#include "tlpi_hdr.h"

const int GIDS_MAX = 2560;

int initgroups_my(const char *user, gid_t group)
{
    // using setgroups() and library functions from ch 8.4
    // initializes calling process's supplementary group IDs by scanning /etc/groups and
    // building a list of all groups of which the named user is a member
    struct group *grp_s;
    size_t gidsetsize = 0;
    gid_t gids[GIDS_MAX]; // the best would be to dynamically allocate by malloc()
    int gids_index = 0;

    while (NULL != (grp_s = getgrent()))
    {
        int mem_index = 0;
        char *name;
        while (NULL != (name = grp_s->gr_mem[mem_index]))
        {
            //
            if (0 == strcmp(name, user))
            {
                //
                if (gids_index < GIDS_MAX)
                {
                    // add group to desired groups list
                    gids[gids_index++] = grp_s->gr_gid;
                }
                else { printf("To much groups. Rewrite the program.\n"); return -1; }
            }

            mem_index++;
        }
    }

    // not needed - not null-terminated list.
    //if (gids_index < GIDS_MAX)
    //    gids[gids_index] = NULL;
    //else { printf("To much groups. Rewrite the program.\n"); return -1; }
    if (gids_index < GIDS_MAX && -1 != group)
    {
        gids[gids_index++] = group;

    }
    else { printf("To much groups. Rewrite the program.\n"); return -1; }
    
    
    // return 0 on success or -1 on error
    return setgroups(gids_index, gids);
}

void print_proces_grps()
{
    //
    gid_t groups[GIDS_MAX];
    int noofgroups = getgroups(GIDS_MAX, groups);
    printf("Proces is in groups: ");
    for (int i = 0; i < noofgroups; ++i)
    {
        //
        printf("%d, ", groups[i]);
    }
    printf("\n");
}

int main(int argc, char** argv)
{
    //
    print_proces_grps();

    initgroups_my(argv[0], -1);
    
    print_proces_grps();

    return EXIT_SUCCESS;
}

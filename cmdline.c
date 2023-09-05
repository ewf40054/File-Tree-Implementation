// Takes input from the command line and runs myfind based off given input
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#include "mytree.h"

int checkarg(char *filetype);
int checkname(char *name);

int main(int argc, char *argv[])
{

    int opt;
    char *name = NULL;
    char *filetype = NULL;
    char *directory = ".";
    int i = 0;
    int perms = 0;
    int size = 0;
    int d = 1000;
    int checker = 0;
    int w = 0;
    int a = 0;
    char *c = NULL;
    char *o = NULL;

    // Reads the command line for -n, -t, and any unknown variables
    while((opt = getopt(argc, argv, "n:t:psd:hwao:c:")) != -1)
    {
        switch(opt)
        {
        	case 'c':
                c = optarg;
                if (realpath(c, NULL) == NULL)
                {
                    perror("Not a valid directory");
                    exit(-1);
                }
                break;
        	case 'o':
                o = optarg;
                int fd = open(o, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if (fd < 0)
                {
                    perror("open");
                    exit(-1);
                }
                if(dup2(fd, 1) < 0)
                {
                    perror("dup2");
                    exit(-1);
                }
                break;
        	case 'a':
                a = 1;
                checker ++;
                break;
        	case 'w':
                w = 1;
                checker ++;
                break;
        	case 'h':
                printf("Usage is the same as tree command in Unix.\n");
                printf("-s provides the size of each file.\n");
                printf("-p provides the permissions of each file.\n");
                printf("-d # allows you to specify the number of subdirectories to look through.");
                printf("\n-n <text> allows you to use the tree to search for a specfic substring.");
                printf("\n-t along with certain characters allows you to specify a file type.");
                printf("\n-w shows the full pathway for the directories and files.\n");
                printf("-a shows hidden files.\n");
                printf("-o allows you to store the output into a specified file.\n");
                printf("-c <directory> allows you to compare between two directories.\n");
                printf("A green word is an executable file.\n");
                printf("A blue word is a directory.\n");
                printf("A red word is a read only file.\n");
                printf("A cyan word is a link to a file.\n");
                exit(0);
        	case 'd':
                d = atoi(optarg);
                if (d <= 0)
                {
                    perror("Value can not be less than or equal to 0");
                    exit(-1);
                }
                break;
        	case 'p':
                perms = 1;
                checker ++;
                break;

        	case 's':
                size = 1;
                checker ++;
                break;
        	case 'n':
                if (checkname(optarg) == 0)
                {
                    name = optarg;
                }
                else
                {
                    perror("Invalid name");
                    exit(-1);
                }
                break;
        	case 't':
                if (checkarg(optarg) == 0)
                {
                    filetype = optarg;
                }
                else
                {
                    perror("Invalid filetype");
                    exit(-1);
                }
                break;
        	case '?':
                perror("Error: Unkown argument");
                exit(-1);
        }
    }

    // Checks for a directory name and fixes the string if a / is already provided
    if (argc > 1 && argc % 2 == 0 && checker % 2 == 0)
    {
        directory = argv[optind];
        int length = strlen(directory);
        if(directory[length - 1] == '/')
        {
            directory[length - 1] = '\0';
        }
    }
    else if (argc > 1 && argc % 2 == 1 && checker % 2 == 1)
    {
        directory = argv[optind];
        int length = strlen(directory);
        if(directory[length - 1] == '/')
        {
            directory[length - 1] = '\0';
        }
    }
    if(directory == NULL)
    {
        perror("Error: Need a directory to search through");
        exit(-1);
    }

    // Checks if using the comparison argument
    if (c == NULL && realpath(directory, NULL) != NULL)
    {
        printf("\033[34m%s\033[0m\n", realpath(directory, NULL));
    }
    else if (realpath(directory, NULL) != NULL)
    {
        printf("\033[34m%-50s\033[0m ", realpath(directory, NULL));
        printf(" \033[34m%s\033[0m\n", realpath(c, NULL));
    }
    else
    {
        perror("Not a valid directory");
        exit(-1);
    }

    // Creates an array to store the file names
    char *all_files[2][4096];
    for (int i = 0; i < 4096; i ++)
    {
        all_files[0][i] = (char *) malloc(512);
        all_files[1][i] = (char *) malloc(512);
    }

    // Searches through the directories
    searchdir(directory, name, filetype, i, perms, size, d, w, 0, a, o, c, all_files[0], 0);
    if (c != NULL)
    {
        searchdir(c, name, filetype, i, perms, size, d, w, 0, a, o, c, all_files[1], 0);
        comparedir(all_files[0], all_files[1]);
    }
    for (int i = 0; i < 4096; i ++)
    {
        free(all_files[0][i]);
        free(all_files[1][i]);
    }
    return 0;
}

// Makes sure the substring is valid in a filename
int checkname(char *name)
{
    int length = strlen(name);
    if (length == 0)
    {
        return -1;
    }
    for(int i = 0; i < length; i ++)
    {
        if(name[i] == '/' || name[i] == '\0')
        {

            return -1;
        }
    }
    for(int i = 0; i < length - 1; i ++)
    {

        if(name[i] == '\\' && name[i + 1] == '0')
        {
            return -1;
        }
    }
    return 0;
}

// Checks for a valid filetype
int checkarg(char *filetype)
{
    if(strlen(filetype) == 1)
    {
        char value = filetype[0];
        if(value == 'b' || value == 'c' || value == 'd' ||
        value == 'f' || value == 'l' || value == 'p' ||
        value == 's')
        {
            return 0;
        }
    }

    return -1;
}

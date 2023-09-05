// Contains the functions to run myfind
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>

#define SUCCESS	0
#define ERROR	-1

int searchdir(char *dirname, char *findme, char *type, int i, int perms, int size, int d, int w
, int lines, int a, char *o, char *c, char *all_files[], int count);
int isdirectory(char * filename);
int checktype(char *filename, char *type);
int print_directory(char *curr, int k, int num_files);
int print_files(char *curr, int perms, int size, int k, int num_files, char *currdir, int a);
int comparedir(char *all_files[], char *other_files[]);

// Compares two directories
int comparedir(char *all_files[], char *other_files[])
{
    int count = 0;
    int count2 = 0;

    while(strlen(all_files[count]) > 0)
    {
        count ++;
    }
    while(strlen(all_files[count2]) > 0)
    {
        count2 ++;
    }

    // Sets space to find matches
    int match_one[count];
    int match_two[count2];

    for (int i = 0; i < count; i ++)
    {
        match_one[i] = 0;
    }
    for (int i = 0; i < count2; i ++)
    {
        match_two[i] = 0;
    }

    // Finds all the matches between the two directories
    int i = 0;
    while(strlen(all_files[i]) > 0)
    {
        char *curr = all_files[i];
        int j = 0;
        //printf("Curr: %s\n", curr);
        while(strlen(other_files[j]) > 0)
        {
            char *curr2 = other_files[j];
            //printf("Curr2: %s\n", curr2);
            if(strcmp(curr, curr2) == 0)
            {
                match_one[i] = 1;
                match_two[j] = 1;
            }
            j ++;
        }
        i ++;
    }

    // There are more files in the second directory // Formatting
    if (count < count2)
    {
        for(int k = 0; k < count; k ++)
        {
            if (match_one[k] == 1)
            {
                printf(" %-50s |", all_files[k]);
            }
            else
            {
                printf(" \033[34m%-50s\033[0m |", all_files[k]);
            }
            if (match_two[k] == 1)
            {
                printf(" %s\n", other_files[k]);
            }
            else
            {
                printf(" \033[34m%s\033[0m\n", other_files[k]);
            }

        }
        for(int k = count; k < count2; k ++)
        {
            printf(" %-50s |", " ");
            if (match_two[k] == 1)
            {
                printf( "%s\n", other_files[k]);
            }
            else
            {
                printf(" \033[34m%s\033[0m\n", other_files[k]);
            }
        }
    }
    // There are equal files or more in the first directory // Formatting
    else
    {
        for(int k = 0; k < count2; k ++)
        {
            if (match_one[k] == 1)
            {
                printf(" %-50s |", all_files[k]);
            }
            else
            {
                printf(" \033[34m%-50s\033[0m |", all_files[k]);
            }
            if (match_two[k] == 1)
            {
                printf(" %s\n", other_files[k]);
            }
            else
            {
                printf(" \033[34m%s\033[0m\n", other_files[k]);
            }
        }
        for(int k = count2; k < count; k ++)
        {
            if (match_one[k] == 1)
            {
                printf(" %-50s |", all_files[k]);
            }
            else
            {
                printf(" \033[34m%-50s\033[0m |", all_files[k]);
            }
            printf( "%s\n", " ");
        }
    }
    return SUCCESS;
} // comparedir




// Checks to make sure a specified name is a directory
int isdirectory(char * filename)
{
    int err;
    struct stat mystat;
    err = lstat(filename, &mystat);
    if(err == -1)
    {
        if (ENOENT == errno)
        {
            return 0; // does not exist
        }
        else
        {
            perror("lstat");
            exit(1);
        }
    }
    else // no error
    {
        if(S_ISDIR(mystat.st_mode))
        {
            return 1; // it is a directory
        }
        else
        {
            printf("ls: %s: No such file or directory", filename);
            return 0;
        }
    }
} // isdirectory

// Recursively searches through directories while printing whatever the specified file type is
int searchdir(char *dirname, char *findme, char *type, int i, int perms, int size, int d, int w
, int lines, int a, char *o, char *c, char *all_files[], int count)
{
    struct dirent **myfile = NULL;
    DIR *mydir;
    char currdir[500];
    strcpy(currdir, dirname);
    char basedir[500];
    int num_files;
    strcpy(basedir, dirname);

    // Makes sure the argument is a directory to look through
    if(!isdirectory(dirname))
    {
        printf("Not a directory\n");
        return 1;
    }

    mydir = opendir(dirname);
    if(mydir == NULL)
    {
        perror("opendir");
        return 1;
    }

    // Checks if user is looking though the current or parent directory
    if (strcmp(dirname, ".") == 0|| strcmp(dirname, "..") == 0)
    {
        // Returns the absolute path of the current or parent directory
        strcpy(basedir, realpath(dirname, NULL));
    }
    // Checks the absolute path if looking from current directory
    if (dirname[0] == '.' && dirname[1] == '/')
    {
        strcpy(basedir, realpath(dirname, NULL));
    }
    // Number of files in current directory
    num_files = scandir(dirname, &myfile, NULL, alphasort);

    // Searches every file in the current directory
    for (int k = 2; k < num_files; k ++)
    {
        strcpy(currdir, basedir);
        char *curr = myfile[k]->d_name;

        // Adjusts the current working directory to have "/"
        if(strcmp(curr, ".") != 0 && strcmp(curr,"..") != 0)
        {
            strcat(currdir, "/");
            strcat(currdir, myfile[k]->d_name);

            // Checks the file type of each file
            if (checktype(currdir, NULL) == 0)
            {
                // Current file is a directory
                if (myfile[k]->d_type == DT_DIR)
                {
                    if(checktype(currdir, NULL) == 0)
                    {
                        // Formatting
                        if (c == NULL)
                        {
                            for (int j = 0; j < lines; j ++)
                            {
                                printf("│ ");
                            }
                            for (int j = lines; j < i; j ++)
                            {
                                printf("  ");
                            }
                        }

                        if(w == 0)
                        {
                            if (c == NULL)
                            {
                                print_directory(curr, k, num_files);
                            }
                            strcpy(all_files[count], curr);
                            count ++;
                        }
                        // Prints the PWD of the current file
                        else
                        {
                            if (c == NULL)
                            {
                                print_directory(currdir, k, num_files);
                            }
                            strcpy(all_files[count], currdir);
                            count ++;
                        }
                    }
                    i ++;

                    // Option if only wanting to look through a certain depth of the tree
                    d --;
                    if (d > 0)
                    {
                        // Determine the depth of the subdirectory
                        if (!(num_files - 1 == k))
                        {
                            lines ++;
                        }
                        // Recursive call
                        searchdir(currdir, findme, type, i, perms, size, d, w, lines, a
                        , o, c, all_files, count);
                        d ++;
                        lines --;
                    }
                    i --;
                }
                else
                {
                    if(findme == NULL)
                    {
                        if(checktype(currdir, type) == 0)
                        {
                            if (c == NULL)
                            {
                                for (int j = 0; j < lines; j ++)
                                {
                                    printf("│ ");
                                }
                                for (int j = lines; j < i; j ++)
                                {
                                    printf("  ");
                                }
                            }

                            // Prints files based on entire pathway or just the name
                            if (w == 0)
                            {
                                if (c == NULL)
                                {
                                    print_files(curr, perms, size, k, num_files, currdir, a);
                                }
                                strcpy(all_files[count], curr);
                                count ++;
                            }
                            else
                            {
                                if (c == NULL)
                                {
                                    print_files(currdir, perms, size, k, num_files, currdir, a);
                                }
                                strcpy(all_files[count], currdir);
                                count ++;
                            }
                        }
                    }
                    // Searches for a specific substring within the directories
                    else if(strstr(curr, findme))
                    {
                        if(checktype(currdir, type) == 0)
                        {
                            if (c == NULL)
                            {
                                for (int j = 0; j < lines; j ++)
                                {
                                    printf("│ ");
                                }
                                for (int j = lines; j < i; j ++)
                                {
                                    printf("  ");
                                }
                            }
                            if (w == 0)
                            {
                                if(c == NULL)
                                {
                                    print_files(curr, perms, size, k, num_files, currdir, a);
                                }
                                strcpy(all_files[count], curr);
                                count ++;
                            }
                            else
                            {
                                if(c ==NULL)
                                {
                                    print_files(currdir, perms, size, k, num_files, currdir, a);
                                }
                                strcpy(all_files[count], currdir);
                                count ++;
                            }
                        }
                    }
                }
            }
        }
    }
    closedir(mydir);
    return SUCCESS;
} // searchdir

// Prints the files with specified colors, permissions, and sizes
int print_files(char *curr, int perms, int size, int k, int num_files, char *path, int a)
{
    struct stat mystat;
    // Does not print hidden files if not specified
    if (a == 0 && curr[0] == '.')
    {
        return 1;
    }

    if(k == num_files - 1)
    {
        printf("└──");
    }
    else
    {
        printf("├──");
    }

    int no_size = 0;
    if(lstat(path, &mystat) == 0 && S_ISLNK(mystat.st_mode))
    {

        printf("\033[36m%s\033[0m", curr);

        char buffer[1024];
        ssize_t link_string_length;
        // Link - Cyan
        if((link_string_length = readlink(path, buffer, sizeof buffer))
        == -1)
        {
            perror("readlink");
        }
        else
        {
            no_size = 1;
            // Make sure that the buffer is terminated as a string
            buffer[link_string_length] = '\0';
            printf(" -> %s", buffer);
        }
    }

    // Executable - Green
    else if(lstat(path, &mystat) == 0 && mystat.st_mode & S_IXUSR)
    {
            printf("\033[32m%s\033[0m", curr);
    }

    // READ ONLY - Red
    else if(
        ((mystat.st_mode & S_IRUSR && !(mystat.st_mode & S_IWUSR || mystat.st_mode & S_IXUSR)))
    && (mystat.st_mode & S_IRGRP && !(mystat.st_mode &S_IWGRP || mystat.st_mode & S_IXGRP))
    && (mystat.st_mode & S_IROTH && !(mystat.st_mode & S_IWOTH || mystat.st_mode & S_IXOTH)))
    {
        printf("\033[31m%s\033[0m", curr);
    } // 0444
    else if(((mystat.st_mode & S_IRUSR && !(mystat.st_mode & S_IWUSR || mystat.st_mode & S_IXUSR)))
    && (mystat.st_mode & S_IRGRP && !(mystat.st_mode &S_IWGRP || mystat.st_mode & S_IXGRP))
    && !(mystat.st_mode & S_IROTH || (mystat.st_mode & S_IWOTH || mystat.st_mode & S_IXOTH)))
    {
        printf("\033[31m%s\033[0m", curr);
    } // 0440
    else if(((mystat.st_mode & S_IRUSR && !(mystat.st_mode & S_IWUSR || mystat.st_mode & S_IXUSR)))
    && !(mystat.st_mode & S_IRGRP || (mystat.st_mode &S_IWGRP || mystat.st_mode & S_IXGRP))
    && !(mystat.st_mode & S_IROTH || (mystat.st_mode & S_IWOTH || mystat.st_mode & S_IXOTH)))
    {
        printf("\033[31m%s\033[0m", curr);
    } // 0400
    else if((!(mystat.st_mode & S_IRUSR || (mystat.st_mode & S_IWUSR || mystat.st_mode & S_IXUSR)))
    && !(mystat.st_mode & S_IRGRP || (mystat.st_mode &S_IWGRP || mystat.st_mode & S_IXGRP))
    && (mystat.st_mode & S_IROTH && !(mystat.st_mode & S_IWOTH || mystat.st_mode & S_IXOTH)))
    {
        printf("\033[31m%s\033[0m", curr);
    } // 0004
    else if(!((mystat.st_mode & S_IRUSR || (mystat.st_mode & S_IWUSR || mystat.st_mode & S_IXUSR)))
    && (mystat.st_mode & S_IRGRP && !(mystat.st_mode &S_IWGRP || mystat.st_mode & S_IXGRP))
    && (mystat.st_mode & S_IROTH && !(mystat.st_mode & S_IWOTH || mystat.st_mode & S_IXOTH)))
    {
        printf("\033[31m%s\033[0m", curr);
    } // 0044
    else if(!((mystat.st_mode & S_IRUSR || (mystat.st_mode & S_IWUSR || mystat.st_mode & S_IXUSR)))
    && (mystat.st_mode & S_IRGRP && !(mystat.st_mode &S_IWGRP || mystat.st_mode & S_IXGRP))
    && !(mystat.st_mode & S_IROTH || (mystat.st_mode & S_IWOTH || mystat.st_mode & S_IXOTH)))
    {
        printf("\033[31m%s\033[0m", curr);
    } // 0040
    else if(((mystat.st_mode & S_IRUSR && !(mystat.st_mode & S_IWUSR || mystat.st_mode & S_IXUSR)))
    && !(mystat.st_mode & S_IRGRP || (mystat.st_mode & S_IWGRP || mystat.st_mode & S_IXGRP))
    && (mystat.st_mode & S_IROTH && !(mystat.st_mode & S_IWOTH || mystat.st_mode & S_IXOTH)))
    {
        printf("\033[31m%s\033[0m", curr);
    } // 0404
    // Default color is white
    else
    {
        printf("%s", curr);
    }
    // Prints permissions in octal mode
    if(perms == 1)
    {
        mode_t mode;
        mode = (mystat.st_mode & 0777);
        printf("  (%o)", mode);
    }
    // Prints the size in kilobytes
    if(size == 1)
    {
        double file_size = (double) mystat.st_size / 1000;
        if (no_size == 0)
        {
            printf("  %1.1fK", file_size);
        }
        no_size = 0;
    }
    printf("\n");
    return SUCCESS;
} // print_files

// Pritns directories with a blue color
int print_directory(char *curr, int k, int num_files)
{
    if (k == num_files - 1)
    {
        printf("└──\033[34m%s\033[0m\n", curr);
    }
    else
    {
        printf("├──\033[34m%s\033[0m\n", curr);
    }
    return SUCCESS;
} // print_direcotory

// Checks the filetype of each file
int checktype(char *filename, char *type)
{
    struct stat mystat;
    int err;

    err = lstat(filename, &mystat);
    if (-1 == err)
    {
        if(ENOENT == errno)
        {

            return -1;
        }
        else
        {
            perror("lstat");
            exit(1);
        }
    }
    else
    {
        if(type == NULL)
        {
            return SUCCESS;
        }
        if(S_ISDIR(mystat.st_mode) && *type == 100)
        {
            // d
            return SUCCESS;
        }
        else if(S_ISREG(mystat.st_mode) && *type == 102)
        {
            // f
            return SUCCESS;
        }
        else if(S_ISCHR(mystat.st_mode) && *type == 99)
        {
            // c
            return SUCCESS;
        }
        else if(S_ISBLK(mystat.st_mode) && *type == 98)
        {
            // b
            return SUCCESS;
        }
        else if(S_ISFIFO(mystat.st_mode) && *type == 112)
        {
            // p
            return SUCCESS;
        }
        else if(S_ISLNK(mystat.st_mode) && *type == 108)
        {
            // l
            return SUCCESS;
        }
        else if(S_ISSOCK(mystat.st_mode) && *type == 115)
        {
            // s
            return SUCCESS;
        }
        else
        {
            return -1;
        }
    }
} // check_type

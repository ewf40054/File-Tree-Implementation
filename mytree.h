// Header file for myfind.c
#ifndef MYFIND_H
#define MYFIND_H

#include <dirent.h>

extern int searchdir(char *dirname, char *findme, char *type, int i, int perms, int size, int d
, int w, int lines, int a, char *o, char *c, char *all_files[], int count);
extern int isdirectory(char *filename);
extern int checktype(char *filename, char *type);
extern int print_directory(char *curr, char *currdir, int w);
extern int print_files(char *curr, int perms, int size);
extern int comparedir(char *all_files[], char *other_files[]);

#endif

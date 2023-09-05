Files:
    mytree.c
    mytree.h
    cmdline.c
    Makefile
    README.txt

How to compile the program:
    Should be compiled using the make command using the Makefile

Usage:
    The usage of this program is to immitate the UNIX tree command that provides an enhanced visual of the given directory structure.

Features:
    -s shows the file sizes in kilobytes
    -p shows the file permissions in octal mode
    -a shows hidden files
    -w shows the absolute path of each file
    -h displays helpful information
    -d <#> specifies how many subdirectories to look through
    -o <filename> writes the output to filename
    -c <directory name> compares the given directory along with the provided directory
    -n <substring> produces the tree with only the provided substring
    -t <filetype> produces the tree with only the specified file types

    Color Code:
    Blue - Directories
    Green - Executable Files
    Red - Read Only Files
    Cyan - Symbolic Links
    White - Anything else

    -c <directory name>:
    Produces the files of each directory side by side, highlighting any files that do not match by name in blue.

Implementation Details:
    Command execution was implemented using getopt(), scandir(), lstat(), dup2().
    getopt() is used to parse the command line and initialize the parameters.
    scandir() is used recursively to look through directories and subdirectories.
    lstat() is used to store information about the files
        st_size is used to get the file sizes
        st_mode is used to find the file permissions
    ANSI escape codes were used to color code the output

Assumptions:
    In order to run the program, you must use ./mytree and provide a directory for the myfind to begin looking. Default is current directory or ".".
       ./mytree <directory-where to start looking>
   In order to run a more advanced version of the program, command line arguments can be added such as

   -n along with a substring to look for certain substrings in filenames. Must be legal file or directory characters
   if -n is not given, the program will look for all file names

   -t along with a filetype such as b, c, d, f, l, p, or s to find a specific filetype
   if -t is not given, all files will be printed

   No size or permission is shown for directories or symbolic links

Known Limitations & Errors:
      Any directory name entered must be a valid directory path or else the program will result in an error.

      If there are command line arguments, they must be followed by the correct value.

      If a substring contains an illegal character, then the program will result in an error.

      If a filetype is not one of the specified types, then the program will result in an error.

      If any command line arguments are used other than ones listed above, then the program will result in an error.

      If the user does not have permissions to access a certain directory, permission will be denied and will result in an error.

      If the value for -d is less than 0 then the program will result in an error.

      If dup2(), lstat(), or scandir() fail, then the program will produce an error.

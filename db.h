/**
Nathen St. Germain

Database reset program.

A program to manage and view the messaging system's database.

CIS * 2750
Assignment 4
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#define HOSTNAME ""
#define USERNAME ""
#define PASSWORD ""
#define DATABASE ""
#define MAX 1024

/**
getCurrentArg()

Get the current command line argument, return NULL when no more
arguments.

param: int, current position, int, total number of args, char **, command line arguments
return: char *, current command line argument
*/
char * getCurrentArg (int pos, int argc, char **argv);

/**
getMaxLen()

Get the greatest length out of the command line arguments.

param: int, number of arguments, char **, command line arguments
return: int, max len out of command line arguments
*/
int getMaxLen (int argc, char **argv);

/**
checkHelpFlag()

Check if the -help flag was included in the arguments.

param: int, number of arguments, char **, command line arguments
return: int, 0 if no help flag, 1 if help flag
*/
int checkHelpFlag (int argc, char **argv);

/**
checkArgs()

Prompt user if the argc value is only one and display choices
that they can choose as arguments since they didn't select any.

param: int, number of command line arguments.
*/
void checkArgs (int argc);

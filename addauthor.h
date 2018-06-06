/**
Nathen St. Germain

Add author program

CIS * 2750
Assignment 2
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
getLenUsername()

Get the length of the user name from the command line.

param: int, number of arguments, char **, arguments
return: int, length of user name
*/
int getLenUsername(int args, char **strs);

/**
checkArgs()

Check if actual arguments equals un wanted amount and prompt/exit if so.

param: int, actual args, int, un wanted number of args
*/
void checkArgs(int actual, int unWanted);

/**
getStreams()

Get streams separated from user and save them in a string.

return: char*, streams separated by commas
*/
char * getStreams();
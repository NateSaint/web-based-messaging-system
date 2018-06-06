/**
Nathen St. Germain

CIS * 2750
Assignment 3
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
parse()

Parse the file and output the HTML to standard out.

param: FILE*, file pointer to parse
*/
void parse(FILE *fp, char *nameOfFile);

/**
getName()

Return the name of the argument of a tag (before equal sign)

param: char*, line, int*, position in line, int, size to malloc
return: char*, char array
*/
char * getName(char *line, int *i, int x);

/**
getDes()

Return the description of the argument of a tag(after equal sign before comma)

param: char*, line, int*, position in line, int, size to malloc
return: char*, char array
*/
char * getDes(char *line, int *i, int x);

/**
checkArguments()

Check if x != y, if so exit prompt and exit program.

param: int x, y, integers to check equivelence of
*/
void checkArguments(int x, int y);

/**
longestPossibleLine()

Get the longest possible line of the file.

param: FILE*, file pointer
return: int,longest possible line
*/
int longestPossibleLine(FILE *fp);

/**
numCommas()

Return the num commas in file.

param: FILE*, file pointer
return: int, num commas
*/
int numCommas(FILE *fp);
/**
Nathen St. Germain

CIS * 2750
Assignment 1
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
parseFile()

Parse the data within file specified by given file name.  Read the file character by character and based
on the character, either add it to the current string in two dimension dynamic array, data, or move to the
next string.  Returns this data in a two dimensional character array once complete.

param: char*, nmae of file to be parsed, int*, number of strings in file
return char**, two dimensional array holding parsed data
*/
char** parseFile(char *fileName, int *numStrings);

/**
addCh()

Different actions based on parameter: int newString.  If newString is 0, char toAdd is added to
currString.  Once newString is 1, concatenate currString to data[*currPos] and start a new string
in data adding char toAdd to the nex string.

param: char**, parsed strings, char, char to add, char*, current string not added yet, int, timesexecuted,
       int*, current position in data, int, whether a new string is to be started.
*/
void addCh(char **data, char toAdd, char *currString, int timesExec, int *currPos, int newString);

/**
isFormatting()

Check if a character is whitespace or formatting.

param: char, character to check
return: int, 1 if true, else, 0
*/
int isFormatting(char c);

/**
isPunctuation()

Check if a character is punctuation.

param: char, character to check
return: int, 1 if true, else, 0
*/
int isPunctuation(char c);

/**
isOperator()

Check if puynctuation is an operator.

param: char, character to check
return: int, 1 if true, else, 0
*/
int isOperator(char c);

/**
freeArray()

Free given two dimensional char array based on number of strings.

param: char**, array to free, int, number of strings within array
*/
void freeArray(char **toFree, int numStrings);
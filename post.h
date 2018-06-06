/**
Nathen St. Germain

User post program

CIS * 2750
Assignment 2
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
getTimeDate()

Get the current time and date and return it as a string.

return: char*, time and date as a string.
*/
char * PostEntrygetTimeDate();

/**
readUserInput()

Read the users input: stream name and text for that stream.

param: char *, username
*/
void PostEntryreadUserInputc(char *username);

/**
formatEntry()

Format the entry given required char arrays as parameters.  Call submit post
function.

param: char*, username, char*, stream name, char*, text of post
*/
void PostEntryformatEntryccc(char *user, char *stream, char *text);

/**
submitPost()

Call the update stream function and free the char* within the struct.

param: struct userPost, information about post.
*/
void PostEntrysubmitPost(struct userPost post);
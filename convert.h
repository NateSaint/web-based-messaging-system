/**
Nathen St. Germain

CIS * 2750
Assignment 1
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct var {
	char *type;
	char *name;
	char *value;
};

typedef struct var var;

struct func {
	int usesClassVar;
	char *oldName;
	char *newName;
	char *entireFunction;
	char *param;
};

typedef struct func func;

struct classData {
	int numVar;
	int numFunc;
	var *variables;
	func *functions;
	char *name;
};

typedef struct classData classData;

/**
convert()

Take the parsed strings in char **data and convert them from C++ lite
to C.  Call for different actions based on the string being analyzed.
Handles major tasks of: 1. Converting classes to structs, 2. Method
renaming within struct, 3. Class scope, 4. Method overloading.  Resulting
.c file is found in the same directory as this file after program execution.

param: char*, name of output file, char**, parsed strings, int, number of strings 
*/
void convert(char *fileName, char **data, int numStrings);

/**
changeExtension()

Change the file extension from .cc to .c.

param: char*, name of file
return: char*, updated extension on file name
*/
char* changeExtension(char *fileName);

/**
getNumClasses()

Get number of classes needed to allocate for classData array.

param: char**, strings, int, number of strings
return: int, number of classes
*/
int getNumClasses(char **data, int numStr);

/**
checkDeclaration()

Check if the class is a declaration.

param: char**, parsed strings, int, current position
return: int, 1 if declaration, else, 0
*/
int checkDeclaration(char **data, int currPos);

/**
isType()

Check if the current string is a type.

param: int, current position, char**, array holding strings
return: int, 1 - inf depending on type, else, 0
*/
int isType(int i, char **data);

/**
skipWhiteSpace()

Skip whitespace by incrementing int, i until no longer whitespace,
return this updated i.

param: int, current position, char**, array holding strings
return: int, updated position in array
*/
int skipWhiteSpace(int i, char **data);

/**
getNumVarFunc()

Get the max number of variables in a class in the .cc file.  Used for
memory allocation purposes.

param: char **, parsed strings of .cc file, int, number of strings
return: max number of variables in a class
*/
int getNumVarFunc(char **data, int numStr);

/**
getNumPointers()

Get the numbner of asterisks in the program.

param: char **, parsed strings, int, number of strings
return: int, number of asterisks
*/
int getNumPointers(char **data, int numStr);

/**
getMaxLen()

Get the maximum length of a string in the 2d arrar.

param: char **, array of strings
return: int, max length
*/
int getMaxLen(char **data, int numStr);

/**
getMaxFunction()

Get the greatest amount of characters in a function.

param: char **, array of strings, int, number of strings
return: int, max number of characters in a function
*/
int getMaxFunction(char **data, int numStr);

/**
checkLocalParam()

Check if a variable is block scope (return 0) or class or global (1)

param: int, pos. in function, char**, array of strings, char*, name of variable to check
return: int, 0 if match, else 1 (means class scope)
*/
int checkLocalParam(int funcPos, char **data, char *varName);
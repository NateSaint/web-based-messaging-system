/**
Nathen St. Germain

Stream library

CIS * 2750
Assignment 2
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

struct userPost {
	char *username;
	char *streamname;
	char *date;
	char *text;
};

/**
updateStream()

All parsed data is passed to the function, updateStream() takes the data
and outputs it to the file updating the <>Stream <>StreamData <>StreamUsers.

param: struct userPost *, struct pointer containing parsed strings from user input.
*/
void updateStream(struct userPost *st);

/**
addUser()

Add user to passed list of streams.

param char*, username to add, char*, list of streams
*/
void addUser(char *username, char *list);

/**
removeUser()

Remove user from passed list of streams.

param: char*, username to remove, char*, list of streams
*/
void removeUser(char *username, char *list);

/**
addToStreamsTable()

Add streamname to the streams table.

param: char *, streamname to add
*/
void addToStreamsTable (char *streamname);

/**
removeFromStreamsTable()

Remove streamname from streams table.

param: char *, streamname to remove
*/
void removeFromStreamsTable (char *streamname);

/**
addToUsersTable()

Add username to the users table.

param: char *, username to add
*/
void addToUsersTable (char *username);

/**
removeFromUsersTable()

Remove username from users table.

param: char *, username to remove
*/
void removeFromUsersTable (char *username);

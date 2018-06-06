#include "stream.h"
#include "post.h"

class PostEntry {
	void readUserInput(char *username) {
		MYSQL db;
       		MYSQL_RES *result;
	        char query[MAX];
		char *streamName, *textInput;

		/*Initialize and connect to database*/
        	mysql_init(&db);
       		mysql_options(&db, MYSQL_READ_DEFAULT_GROUP, "mydb");

	        mysql_real_connect(&db, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0);

		/*Get stream name and post text from file "posttext"*/
		FILE *file;
		int max = 3, x = 0;
		char c;

		file = fopen("posttext", "r");
		while ((c = fgetc(file)) != EOF) max++;
		fclose(file);

		streamName = malloc(sizeof(char) * max);
		textInput = malloc(sizeof(char) * max);
		streamName[0] = '\0';
		textInput[0] = '\0';

		int changed = 0;
		file = fopen("posttext", "r");
		while ((c = fgetc(file)) != EOF)
		{
			int len;
			if (c == '\n') x = 1;
			if (x == 0)
			{
				len = strlen(streamName);
				streamName[len] = c;
				streamName[len + 1] = '\0';
			}
			else if (x == 1 && changed != 0)
			{
				len = strlen(textInput);
				textInput[len] = c;
				textInput[len + 1] = '\0';
			}
			if (x == 1) changed = 1;
		}

		/*
		Check existence of stream name, if exists check if username is allowed to post
		*/

		/*check streamname*/
		int permission = 0;
		int streamExists = 0;
		char *name;
		name = malloc(sizeof(char) * (strlen(streamName) + 50));
                name[0] = '\0';
                strcat(name, streamName);

                query[0] = '\0';
                strcat(query, "show tables like '");
                strcat(query, name);
                strcat(query, "StreamUsers'");

	        mysql_query(&db, query);
                result = mysql_store_result(&db);

		if (result == NULL)
			streamExists = 1;
                else if (mysql_num_rows(result) == 0)
                        streamExists = 1;

		mysql_free_result(result);

		if (streamExists == 1)
                        printf("Stream %s does not exist, cannot post.<br>", name);
                /*Stream exists... check if sender exists*/
                else
                {
                        /*CHECK IF USER IS A PART OF StreamUsers Already, if so delete them*/
                        query[0] = '\0';
                        strcat(query, "select 1 from `");
                        strcat(query, name);
                        strcat(query, "StreamUsers` where user=\"");
                        strcat(query, username);
                        strcat(query, "\"");
                        mysql_query(&db, query);
                        result = mysql_store_result(&db);
                        if (mysql_num_rows(result) == 0)
                        {
                        	printf("User %s does not have permission to post in stream, '%s'.<br>", username, name);
                        }
                        else
                        {
                                permission = 1;
                        }
			mysql_free_result(result);
                }
		mysql_close(&db);

		free(name);
		/*REMOVE END HERE*/

		/*CALL NEXT FNC WITH STREAMNAME*/
		if (permission == 1)
			PostEntryformatEntryccc(username, streamName, textInput);

		free(streamName);
		free(textInput);
	}

	char * getTimeDate() {
		time_t timeNow;
		timeNow = time(NULL);
		return ctime(&timeNow);
	}

	void formatEntry(char *user, char *stream, char *text) {
		struct userPost post;

		post.username = malloc(sizeof(char) * strlen(user));
		post.streamname = malloc(sizeof(char) * strlen(stream));
		post.date = malloc(sizeof(char) * strlen(PostEntrygetTimeDate()));
		post.text = malloc(sizeof(char) * strlen(text));

		strcpy(post.username, user);
		strcpy(post.streamname, stream);
		strcpy(post.date, PostEntrygetTimeDate());
		strcpy(post.text, text);

		/*Remove new line from date*/
		post.date[strlen(post.date) - 1] = '\0';

		/*Call for the struct to be added*/
		PostEntrysubmitPost(post);

		free(post.username);
		free(post.streamname);
		free(post.date);
		free(post.text);
	}

	void submitPost(struct userPost post) {
		updateStream(&post);
	}
};

int main(int argc, char **argv)
{
	class PostEntry post;
	char *username, *temp;

	checkArgs(argc, 1);

	username = malloc(sizeof(char) * getLenUsername(argc, argv));
	temp = malloc(sizeof(char) * getLenUsername(argc, argv));
	username[0] = '\0';

	/*Concatenate command line arguments into a single string*/
	int i;
	for (i = 1; i < argc; i++)
	{
		memset(temp, 0, strlen(temp));
		strcpy(temp, argv[i]);
		strcat(username, temp);

		if (argc != i + 1)
			strcat(username, " ");
	}

	post.readUserInput(username);

	free(username);
	free(temp);
	return 0;
}

int getLenUsername(int args, char **strs)
{
	int i, len = args, currLen;

	for (i = 1; i < args; i++)
	{
		currLen = strlen(strs[i]);
		len += currLen;
	}

	return len;
}

void checkArgs(int actual, int unWanted)
{
	if (actual == unWanted)
	{
		printf("Incorrect command line arguments, please enter the author ID\nPost will now exit . . .");
		exit(0);
	}
}

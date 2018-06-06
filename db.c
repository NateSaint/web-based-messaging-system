#include "db.h"

int main (int argc, char **argv)
{
	MYSQL db;
	MYSQL_RES *res;
	MYSQL_ROW row;
        char query[MAX];
	int help;
	int currentPos;
	int maxLen;
	char *currentArg;

	/*Initialize and connect to database*/
        mysql_init(&db);
        mysql_options(&db, MYSQL_READ_DEFAULT_GROUP, "mydb");
        mysql_real_connect(&db, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0);

	/*Initialize variables for use with program*/
	checkArgs(argc);
	help = checkHelpFlag(argc, argv);
	maxLen = getMaxLen(argc, argv);
	currentArg = malloc(sizeof(char) * maxLen);
	currentArg[0] = '\0';
	currentPos = 1;

	if (help == 1)
	{
		printf("-----\nHelp\n-----\n");
		printf("Available flags are:\n-clear (clear data from all tables)\n-reset (remove all tables)\n-posts (print all posts)\n-users (print all users)\n-streams (print all streams)\n");
                printf("\nIf multiple flags are specified, the program will execute them in order\n");
                printf("\nHelp flag is now active so aditional information may be displayed on following flags...\n");
		printf("------------\nEnd of help\n------------\n\n");
	}

	/*Loop through each command line argument*/
	while ((currentArg = getCurrentArg(currentPos, argc, argv)) != NULL)
	{
		if (strcmp(currentArg, "-clear") == 0)
		{
			int atleastOne = 0;

			if (help == 1) printf("\"-clear\" flag selected, remove data from tables\n");
			query[0] = '\0';
			strcat(query, "show tables");
                        mysql_query(&db, query);
                        res = mysql_store_result(&db);

			while ((row = mysql_fetch_row(res)))
                        {
				/*if (strcmp(row[0], "streams") != 0){*/
                                atleastOne = 1;
                                query[0] = '\0';
                                strcat(query, "delete from `");
                                strcat(query, row[0]);
                                strcat(query, "`");
                                mysql_query(&db, query);
                                printf("Cleared table: \"%s\"\n", row[0]);
				/*}*/
                        }
                        if (atleastOne == 0) printf("No tables to clear\n");
			else printf("Done clearing tables\n");
		}
		else if (strcmp(currentArg, "-reset") == 0)
		{
			int atleastOne = 0;

			if (help == 1) printf("\"-reset\" flag selected, delete tables form database\n");
			query[0] = '\0';
			strcat(query, "show tables");
			mysql_query(&db, query);
			res = mysql_store_result(&db);

			while ((row = mysql_fetch_row(res)))
			{
				atleastOne = 1;
				query[0] = '\0';
				strcat(query, "drop table `");
				strcat(query, row[0]);
				strcat(query, "`");
				mysql_query(&db, query);
				printf("Dropped table: \"%s\"\n", row[0]);
			}
			if (atleastOne == 0) printf("No tables to remove\n");
			else printf("Done removing tables\n");
		}
		else if (strcmp(currentArg, "-posts") == 0)
                {
			int atleastOne = 0;
			int printOnce = 0;

			if (help == 1) printf("\"-posts\" flag selected, print all posts stored in database\n");

			query[0] = '\0';
                        strcat(query, "show tables like '%Stream'");
                        mysql_query(&db, query);
                        res = mysql_store_result(&db);

                        while ((row = mysql_fetch_row(res)))
                        {
				MYSQL_ROW row1;
				MYSQL_RES *res1;
                                query[0] = '\0';
                                strcat(query, "select * from `");
                                strcat(query, row[0]);
                                strcat(query, "`");
                                mysql_query(&db, query);
				res1 = mysql_store_result(&db);

				while ((row1 = mysql_fetch_row(res1)))
				{
					if (printOnce == 0) {printOnce = 1; printf("Posts:\n");}
					atleastOne = 1;
					int len = strlen(row[0]) - 6;
					printf("Stream: %.*s\nSender: %s\nDate: %s\n%s\n\n", len, row[0], row1[0], row1[1], row1[2]);
				}
                        }
                        if (atleastOne == 0) printf("No posts to print\n");
                }
		else if (strcmp(currentArg, "-users") == 0)
                {
			int atleastOne = 0;
			if (help == 1) printf("\"-users\" flag selected, print all user names stored in database\n");
			query[0] = '\0';
                        strcat(query, "select * from users");
                        if (mysql_query(&db, query)){} else {
                        	res = mysql_store_result(&db);

				int printOnce = 0;
                        	while ((row = mysql_fetch_row(res)))
                        	{
					if (printOnce == 0) {printOnce = 1; printf("Users:\n");}
                                	atleastOne = 1;
                                	printf("%s\n", row[0]);
                        	}
			}
                        if (atleastOne == 0) printf("No users to print\n");
			else printf("\n");
                }
		else if (strcmp(currentArg, "-streams") == 0)
                {
			int atleastOne = 0;
			if (help == 1) printf("\"-streams\" flag selected, print all stream names stored in database\n");
			query[0] = '\0';
                        strcat(query, "select * from streams");
                        if (mysql_query(&db, query)){} else {
                        	res = mysql_store_result(&db);

				int printOnce = 0;
                        	while ((row = mysql_fetch_row(res)))
                        	{
					if (printOnce == 0) {printOnce = 1; printf("Streams:\n");}
                                	atleastOne = 1;
                                	printf("%s\n", row[0]);
                        	}
			}
                        if (atleastOne == 0) printf("No streams to print\n");
			else printf("\n");
                }
		else if (strcmp(currentArg, "-help") == 0)
		{
			/*Do nothing*/
		}
		/*
		THESE ARE CUSTOM FLAGS FOR MY PYTHON PROGRAMS WORKING WITH MYSQL IN C
		*/
		else if (strcmp(currentArg, "-checkLogin") == 0)
		{
			/*Check if the user exists in users table*/
			FILE *fp = fopen("temp", "w");
			int exists = 0;

			currentPos++;

			/*Check if user exists*/
			query[0] = '\0';
			strcat(query, "select 1 from users where username=\"");
			strcat(query, argv[currentPos]);
			strcat(query, "\"");

			if (mysql_query(&db, query))
				exists = 0;
			else
			{
				res = mysql_store_result(&db);
				if (mysql_num_rows(res) == 1)
					exists = 1;
			}

			fprintf(fp, "%d", exists);
			fclose(fp);
		}
		else if (strcmp(currentArg, "-getStreams") == 0)
		{
			/*Get the streams that the user is a part of*/
			currentPos++;

			/*Check for streams that contain username*/
			int atleastOne = 0;
                        if (help == 1) printf("\"-streams\" flag selected, print all stream names stored in database\n");
                        query[0] = '\0';
                        strcat(query, "select * from streams");
                        if (mysql_query(&db, query)){} else {
                                res = mysql_store_result(&db);

                                while ((row = mysql_fetch_row(res)))
                                {
					/*check if user is a part of stream, if so print*/
					query[0] = '\0';
					strcat(query, "select * from `");
					strcat(query, row[0]);
					strcat(query, "StreamUsers` where user=\"");
					strcat(query, argv[currentPos]);
					strcat(query, "\"");

					if (mysql_query(&db, query)){} else {
						/*User has permission to view stream*/
                                        	MYSQL_RES *res1 = mysql_store_result(&db);
		                                if (mysql_num_rows(res1) > 0) {
                                        		atleastOne = 1;
                                        		printf("%s<br>", row[0]);
						}
					}
                                }
                        }
                        if (atleastOne == 0) printf("No streams to display for user, %s.<br>", argv[currentPos]);
		}
		else if (strcmp(currentArg, "-verifyStream") == 0)
		{
			/*Ensure that stream exists*/
			int atleastOne = 0;
			FILE *fp = fopen("temp", "w");
			currentPos++;

			query[0] = '\0';
                        strcat(query, "select * from streams");
                        if (mysql_query(&db, query)){} else {
                                res = mysql_store_result(&db);

                                while ((row = mysql_fetch_row(res)))
                                {
					/*If there is a match*/
                                        if (strcmp(argv[currentPos], row[0]) == 0)
					{
                                        	atleastOne = 1;
                                        	fprintf(fp, "1");
					}
                                }
                        }
                        if (atleastOne == 0) fprintf(fp, "0");
			fclose(fp);
		}
		else if (strcmp(currentArg, "-getLastUnread") == 0)
		{
			/*get the last unread message by a specific user*/
			int atleastOne = 0;
                        FILE *fp = fopen("temp", "w");
                        currentPos++;

                        query[0] = '\0';
                        strcat(query, "select postsViewed from `");
			strcat(query, argv[currentPos]);
			strcat(query, "StreamUsers` where user=\"");
			strcat(query, argv[currentPos + 1]);
			strcat(query, "\"");
                        if (mysql_query(&db, query)){} else {
                                res = mysql_store_result(&db);
				if (mysql_num_rows(res) > 0) {
                                	row = mysql_fetch_row(res);
					fprintf(fp, "%s", row[0]);
					atleastOne = 1;
				}
                        }
                        if (atleastOne == 0) fprintf(fp, "-1");
                        fclose(fp);
			currentPos++;
		}
		else if (strcmp(currentArg, "-getMax") == 0)
		{
			/*Get the number of posts in specified stream*/
			FILE *fp = fopen("temp", "w");

			currentPos++;
			query[0] = '\0';
			strcat(query, "select count(*) from `");
			strcat(query, argv[currentPos]);
			strcat(query, "Stream`");

			if (mysql_query(&db, query)){fprintf(fp, "-1");} else {
				res = mysql_store_result(&db);
				row = mysql_fetch_row(res);
				fprintf(fp, "%s", row[0]);
			}

			fclose(fp);
		}
		else if (strcmp(currentArg, "-printPost") == 0)
		{
			/*Print the current post in specified stream and index*/
			FILE *fp = fopen("temp", "w");

			currentPos++;
			query[0] = '\0';
			strcat(query, "select * from `");
			strcat(query, argv[currentPos]);
			strcat(query, "Stream` where id=\"");
			strcat(query, argv[currentPos + 1]);
			strcat(query, "\"");
			currentPos++;

			if (mysql_query(&db, query)) {} else {
				res = mysql_store_result(&db);
                                row = mysql_fetch_row(res);
                                fprintf(fp, "Sender: %s\nDate: %s\n%s\n", row[0], row[1], row[2]);
			}

			fclose(fp);
		}
		else if (strcmp(currentArg, "-updateViewed") == 0)
		{
			/*Update the viewed messages on specified user, stream and amount*/
			currentPos++;
			query[0] = '\0';
			strcat(query, "update `");
			strcat(query, argv[currentPos]);/*Stream*/
			strcat(query, "StreamUsers` set postsViewed=");
			strcat(query, argv[currentPos + 1]);/*Value to change*/
			strcat(query, " where user=\"");
			strcat(query, argv[currentPos + 2]);/*Username*/
			strcat(query, "\"");
			currentPos += 2;
			mysql_query(&db, query);
		}
		else
			printf("Unrecognized flag \"%s\"\n\n", currentArg);

		currentPos++;
	}

	free(currentArg);
	mysql_close(&db);

	return 0;
}

char * getCurrentArg (int pos, int argc, char **argv)
{
	if (pos == argc) return NULL;
	else return argv[pos];
}

int getMaxLen (int argc, char **argv)
{
	int maxLen = 0;
	int i;
	int max = 0;
	for (i = 0; i < argc; i++)
		if (strlen(argv[i]) > max)
			max = strlen(argv[i]);
	return maxLen + 5;
}

int checkHelpFlag (int argc, char **argv)
{
	int hasHelpFlag = 0;
	int i;

	for (i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-help") == 0)
		{
			hasHelpFlag = 1;
			break;
		}
	}

	return hasHelpFlag;
}

void checkArgs (int argc)
{
	if (argc == 1)
	{
		printf("No arguments specified, available arguments are:\n-clear\n-reset\n-posts\n-users\n-streams\n-help\n");
		exit(0);
	}
}

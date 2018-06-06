#include "stream.h"

void updateStream(struct userPost *st)
{
	MYSQL db;
        char query[MAX];
	char *temp;

	/*Initialize and connect to database*/
        mysql_init(&db);
        mysql_options(&db, MYSQL_READ_DEFAULT_GROUP, "mydb");

        mysql_real_connect(&db, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0);


	temp = malloc(sizeof(char) * (strlen(st->username) + strlen(st->streamname) + strlen(st->date) + strlen(st->text) + 25));
	memset(temp, 0, strlen(st->username) + strlen(st->streamname) + strlen(st->date) + strlen(st->text) + 25);
	temp[0] = '\0';

	/*
	Create tables if needed for stream.
	*/
	/*Create tables for Stream, StreamUsers, StreamData if they do not exist*/
	char *name = malloc(sizeof(char) * (strlen(st->username) + strlen(st->streamname) + strlen(st->date) + strlen(st->text) + 25));
	name[0] = '\0';
        strcat(name, st->streamname);

        query[0] = '\0';
        strcat(query, "create table if not exists `");
        strcat(query, name);
        strcat(query, "Stream` (sender char(100), date char(50), post varchar(4000), id int not null auto_increment, primary key(id))");
        mysql_query(&db, query);

        query[0] = '\0';
        strcat(query, "create table if not exists `");
        strcat(query, name);
        strcat(query, "StreamUsers` (user char(100), postsViewed int)");
        mysql_query(&db, query);

	addToStreamsTable(name);

	/*Create query*/
	query[0] = '\0';
	strcat(query, "insert into `");
	strcat(query, st->streamname);
	strcat(query, "Stream` (sender, date, post) values (\"");
	strcat(query, st->username);
	strcat(query, "\", \"");
	strcat(query, st->date);
	strcat(query, "\", \"");
	strcat(query, st->text);
	strcat(query, "\")");

	/*Call query in sql*/
	mysql_query(&db, query);

	printf("Message posted in stream, %s, by user, %s.<br>", st->streamname, st->username);

	mysql_close(&db);

	free(temp);
	free(name);
}

void addUser(char *username, char *list)
{
	MYSQL db;
	MYSQL_RES *result;
	char query[MAX];
	char *temp, *name;

	/*Initialize and connect to database*/
	mysql_init(&db);
	mysql_options(&db, MYSQL_READ_DEFAULT_GROUP, "mydb");

	mysql_real_connect(&db, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0);

	temp = malloc(sizeof(char) * (strlen(list) + 1));
	temp[0] = '\0';

	name = malloc(sizeof(char) * (strlen(list) + 25));
	name[0] = '\0';

	/*Separate each stream name and handle appropriately*/
	int i;
	for (i = 0; i < strlen(list); i++)
	{
		/*Stop adding to temp*/
		if (list[i] == ',' || i == strlen(list) - 1)
		{
			if (i == strlen(list) - 1);
			{
				if (list[i] != ',')
				{
					int x = strlen(temp);
                                	temp[x] = list[i];
                                	temp[x + 1] = '\0';
				}
			}

			/*
				Create new files if needed
			*/

			/*Create tables for Stream, StreamUsers if they do not exist*/
			strcat(name, temp);

			query[0] = '\0';
			strcat(query, "create table if not exists `");
			strcat(query, name);
			strcat(query, "Stream` (sender char(100), date char(50), post varchar(4000), id int not null auto_increment, primary key(id))");
			mysql_query(&db, query);

			query[0] = '\0';
                        strcat(query, "create table if not exists `");
                        strcat(query, name);
                        strcat(query, "StreamUsers` (user char(100), postsViewed int)");
                        mysql_query(&db, query);

			addToStreamsTable(name);

			/*CHECK IF USER IS A PART OF StreamUsers Already, if not add them*/
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
				/*Adding username to *StreamUsers*/
				query[0] = '\0';
				strcat(query, "insert into `");
				strcat(query, name);
				strcat(query, "StreamUsers` values (\"");
				strcat(query, username);
				strcat(query, "\", 0)");
				mysql_query(&db, query);

				printf("Added %s to stream, '%s'.<br>", username, name);
				addToUsersTable(username);
			}
			else
				printf("User %s already exists in stream, '%s'.<br>", username, name);

			memset(name, 0, strlen(name));
			name[0] = '\0';

			memset(temp, 0, strlen(temp));
			temp[0] = '\0';
		}
		/*Add char to temp*/
		else
		{
			int length = strlen(temp);
			temp[length] = list[i];
			temp[length + 1] = '\0';
		}
	}

	mysql_close(&db);

	free(temp);
	free(name);
}

void removeUser(char *username, char *list)
{
	MYSQL db;
        MYSQL_RES *result;
        char query[MAX];
	char *temp, *name;

	/*Initialize and connect to database*/
        mysql_init(&db);
        mysql_options(&db, MYSQL_READ_DEFAULT_GROUP, "mydb");

        mysql_real_connect(&db, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0);

	temp = malloc(sizeof(char) * (strlen(list) + 1));
	temp[0] = '\0';
	name = malloc(sizeof(char) * (strlen(list) + 30));
	name[0] = '\0';

	/*Separate each stream name and handle appropriately*/
	int i;
	for (i = 0; i < strlen(list); i++)
	{
		/*Stop adding to temp*/
		if (list[i] == ',' || i == strlen(list) - 1)
		{
			if (i == strlen(list) - 1 && list[i] != ',')
			{
				int x = strlen(temp);
				temp[x] = list[i];
				temp[x + 1] = '\0';
			}

			/*
			Remove user from stream if it exists
			*/

			/*Check existence of stream*/
			int streamExists = 0;
			name[0] = '\0';
			strcat(name, temp);

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


			if (streamExists == 1)
				printf("Stream %s does not exist, cannot remove user.<br>", name);
			/*Stream exists... Remove author from the stream*/
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
                                	printf("User %s does not exist in stream, '%s'.<br>", username, name);
                       		}
                        	else
                        	{
					query[0] = '\0';
					strcat(query, "delete from `");
					strcat(query, name);
					strcat(query, "StreamUsers` where user=\"");
					strcat(query, username);
					strcat(query, "\"");
					mysql_query(&db, query);

					printf("Removed user, %s, from stream, '%s'.<br>", username, name);
					removeFromUsersTable(username);
				}
			}

			memset(name, 0, strlen(name));
			name[0] = '\0';
			memset(temp, 0, strlen(temp));
			temp[0] = '\0';
		}
		/*Add char to temp*/
		else
		{
			int length = strlen(temp);
			temp[length] = list[i];
			temp[length + 1] = '\0';
		}
	}

	free(temp);
	free(name);

	mysql_close(&db);
}

void addToStreamsTable (char *streamname)
{
	MYSQL db;
        char query[MAX];

	/*Initialize and connect to database*/
        mysql_init(&db);
        mysql_options(&db, MYSQL_READ_DEFAULT_GROUP, "mydb");
        mysql_real_connect(&db, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0);

	/*create table if needed*/
	query[0] = '\0';
        strcat(query, "create table if not exists `streams` (streamname char(200))");
        mysql_query(&db, query);

	query[0] = '\0';
        strcat(query, "select 1 from `");
        strcat(query, "streams");
        strcat(query, "` where streamname=\"");
        strcat(query, streamname);
        strcat(query, "\"");

        mysql_query(&db, query);
        MYSQL_RES *result = mysql_store_result(&db);
        if (mysql_num_rows(result) == 0)
        {
        	/*insert into streams*/
        	query[0] = '\0';
        	strcat(query, "insert into `streams` values (\"");
        	strcat(query, streamname);
       		strcat(query, "\")");
        	mysql_query(&db, query);
        }

	mysql_close(&db);
}

void removeFromStreamsTable (char *streamname)
{
	MYSQL db;
        char query[MAX];

        /*Initialize and connect to database*/
        mysql_init(&db);
        mysql_options(&db, MYSQL_READ_DEFAULT_GROUP, "mydb");
        mysql_real_connect(&db, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0);

	/*create table if needed*/
        query[0] = '\0';
        strcat(query, "create table if not exists `users` (username char(200))");
        mysql_query(&db, query);

        /*Remove from streams*/
        query[0] = '\0';
        strcat(query, "delete from `streams` where streamname=\"");
        strcat(query, streamname);
        strcat(query, "\"");
	mysql_query(&db, query);

        mysql_close(&db);
}

void addToUsersTable (char *username)
{
        MYSQL db;
        char query[MAX];

        /*Initialize and connect to database*/
        mysql_init(&db);
        mysql_options(&db, MYSQL_READ_DEFAULT_GROUP, "mydb");
        mysql_real_connect(&db, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0);

        /*create table if needed*/
        query[0] = '\0';
        strcat(query, "create table if not exists `users` (username char(200))");
        mysql_query(&db, query);

	query[0] = '\0';
        strcat(query, "select 1 from `");
        strcat(query, "users");
        strcat(query, "` where username=\"");
        strcat(query, username);
        strcat(query, "\"");

        mysql_query(&db, query);
        MYSQL_RES *result = mysql_store_result(&db);
        if (mysql_num_rows(result) == 0)
        {
        	/*insert into streams*/
        	query[0] = '\0';
        	strcat(query, "insert into `users` values (\"");
        	strcat(query, username);
        	strcat(query, "\")");
		mysql_query(&db, query);
	}

        mysql_close(&db);
}

void removeFromUsersTable (char *username)
{
        MYSQL db;
	MYSQL_ROW row;
	MYSQL_RES *res;
        char query[MAX];

        /*Initialize and connect to database*/
        mysql_init(&db);
        mysql_options(&db, MYSQL_READ_DEFAULT_GROUP, "mydb");
        mysql_real_connect(&db, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0);

	/*create table if needed*/
        query[0] = '\0';
        strcat(query, "create table if not exists `users` (username char(200))");
        mysql_query(&db, query);

	/*check if user exists in more than one table*/
	query[0] = '\0';
        strcat(query, "show tables like '%StreamUsers'");
        mysql_query(&db, query);
        res = mysql_store_result(&db);
	int numName = 0;
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
			if (strcmp(row1[0], username) == 0)
			{
				numName++;
			}
                }
        }

	if (numName < 2) {
        	/*Remove from users*/
        	query[0] = '\0';
        	strcat(query, "delete from `users` where username=\"");
        	strcat(query, username);
        	strcat(query, "\"");
		mysql_query(&db, query);
	}

        mysql_close(&db);
}

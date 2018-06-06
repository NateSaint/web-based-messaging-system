#include "stream.h"
#include "addauthor.h"

int main(int argc, char **argv)
{
	char *username, *temp, *streams;
	int removeUsername;

	removeUsername = 0;
	checkArgs(argc, 1);

	username = malloc(sizeof(char) * getLenUsername(argc, argv));
	temp = malloc(sizeof(char) * getLenUsername(argc, argv));
	username[0] = '\0';

	/*Concatenate command line arguments into a single string and deal with -r argument*/
	int i;
	for (i = 1; i < argc; i++)
	{
		memset(temp, 0, strlen(temp));
		strcpy(temp, argv[i]);

		if (strcmp(temp, "-r") == 0 && removeUsername == 0)
			removeUsername = 1;
		else
		{
			strcat(username, temp);

			if (argc != i + 1)
				strcat(username, " ");
		}
	}

	streams = getStreams();
	if (strcmp(streams, "") == 0)
        {
                printf("Empty stream name.<br>");
                return 0;
        }
	/*Based on -r flag, remove the username or add it*/
	if (removeUsername == 1)
		removeUser(username, streams);
	else if (removeUsername == 0)
		addUser(username, streams);

	free(username);
	free(temp);
	free(streams);
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
		printf("Incorrect author format.<br>");
		exit(0);
	}
}

char * getStreams()
{
	char * streams, c;

	streams = malloc(sizeof(char) * 10000);
	streams[0] = '\0';

	FILE *fp = fopen("addremovetext", "r");

	while ((c = fgetc(fp)) != EOF)
	{
		int len = strlen(streams);
		streams[len] = c;
		streams[len + 1] = '\0';
	}
	fclose(fp);
	return streams;
}

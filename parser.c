#include "parser.h"

char** parseFile(char *fileName, int *numStrings)
{
	int numStr, timesExec, currLen, maxLen, currPos;
	char **data;
	char *currString;
	FILE *file;

	file = fopen(fileName, "r");

	if (file == NULL)
	{
		printf("Error when trying to open file. [FILE NULL]\n");
		printf("Program will now exit.\n");
		exit(0);
	}

	/*Default variable initialization*/
	timesExec = 0;
	currLen = 0;
	maxLen = 0;
	currPos = 0;

	while (timesExec < 2)
	{
		char ch;

		if (timesExec == 1)
		{
			int i;
			
			/*Allocate memory for data*/
			data = malloc(sizeof(char*) * numStr);
			for (i = 0; i < numStr; i++)
				data[i] = malloc(sizeof(char) * maxLen);
			currString = malloc(sizeof(char) * maxLen);

			rewind(file);
		}

		numStr = 1;

		/*Read and analyze file one char at a time*/
		while ((ch = getc(file)) != EOF)
		{
			if (isFormatting(ch) || isPunctuation(ch) || isOperator(ch))
			{
				addCh(data, ch, currString, timesExec, &currPos, 1);
				currLen = 0;
				numStr += 2;
			}
			else if (ch == '/')
			{
				char nextCh = getc(file);

				addCh(data, 0, currString, timesExec, &currPos, 1);
				addCh(data, ch, currString, timesExec, &currPos, 0);
				currLen = 1;
				numStr++;

				/*Single line comment*/
				if (nextCh == '/')
				{
					addCh(data, nextCh, currString, timesExec, &currPos, 0);
					currLen += 2;
					
					while ((nextCh = getc(file)) != '\n' && nextCh != '\r' && nextCh != '\0')
					{
						addCh(data, nextCh, currString, timesExec, &currPos, 0);
						currLen++;
					}
					addCh(data, nextCh, currString, timesExec, &currPos, 1);
					numStr++;
				}
				/*Multiline comment*/
				else if (nextCh == '*')
				{
					char nextCh1;
					int end = 0;
					addCh(data, nextCh, currString, timesExec, &currPos, 0);
					currLen += 2;

					do
					{
						nextCh = getc(file);
						nextCh1 = getc(file);
						if (nextCh == '*' && nextCh1 == '/')
						{
							end = 1;
							addCh(data, nextCh, currString, timesExec, &currPos, 0);
							addCh(data, nextCh1, currString, timesExec, &currPos, 0);
							currLen += 3;
						}
						else
						{
							ungetc(nextCh1, file);
							addCh(data, nextCh, currString, timesExec, &currPos, 0);
							currLen++;
						}
					} while (end == 0);
				}
				/*Operator '/'*/
				else
				{
					addCh(data, 0, currString, timesExec, &currPos, 1);
					numStr += 2;
					ungetc(nextCh, file);
				}
			}
			else if (ch == '"')
			{
				char nextCh;
				addCh(data, 0, currString, timesExec, &currPos, 1);
				addCh(data, ch, currString, timesExec, &currPos, 0);
				currLen++;

				while ((nextCh = getc(file)) != '"')
				{
					if (nextCh == '\\')
					{
						addCh(data, nextCh, currString, timesExec, &currPos, 0);
						addCh(data, getc(file), currString, timesExec, &currPos, 0);
						currLen += 2;
					}
					else
					{
						addCh(data, nextCh, currString, timesExec, &currPos, 0);
						currLen++;
					}
				}

				addCh(data, nextCh, currString, timesExec, &currPos, 0);
				addCh(data, 0, currString, timesExec, &currPos, 1);
				currLen += 2;
				numStr += 2;
			}
			else
			{
				char temp;

				/*Force character at end of file not followed by whitespace to get a string*/
				if ((temp = getc(file)) == EOF)
				{
					addCh(data, ch, currString, timesExec, &currPos, 0);
					addCh(data, ' ', currString, timesExec, &currPos, 1);
				}
				else
				{	
					ungetc(temp, file);
					addCh(data, ch, currString, timesExec, &currPos, 0);
				}
				currLen++;
			}

			if (currLen > maxLen)
				maxLen = currLen;
		}

		timesExec++;
	}

	*numStrings = numStr;
	free(currString);
	fclose(file);
	return data;
}

void addCh(char **data, char toAdd, char *currString, int timesExec, int *currPos, int newString)
{
	int lenCurrStr;

	/*Return if in the counting strings needed stage*/
	if (timesExec < 1)
		return;

	lenCurrStr = strlen(currString);

	/*Add currString to array and toAdd and then start a new one*/
	if (newString == 1)
	{
		if (lenCurrStr != 0)
		{
			strcpy(data[*currPos], currString);
			*currPos = *currPos + 1;
		}

		data[*currPos][0] = toAdd;
		*currPos = *currPos + 1;

		/*Reset currString*/
		memset(currString, 0, strlen(currString));
	}
	/*Add toAdd to end of currString*/
	else
		currString[lenCurrStr] = toAdd;
}

int isFormatting(char c)
{
	if (c == ' ') return 1;
	else if (c == '\n') return 1;
	else if (c == '\t') return 1;
	else if (c == '\r') return 1;
	else if (c == '\0') return 1;
	else return 0;
}

int isPunctuation(char c)
{
	if (c == '.') return 1;
	else if (c == '<') return 1;
	else if (c == '>') return 1;
	else if (c == ',') return 1;
	else if (c == ';') return 1;
	else if (c == '(') return 1;
	else if (c == ')') return 1;
	else if (c == '{') return 1;
	else if (c == '}') return 1;
	else return 0;
}

int isOperator(char c)
{
	if (c == '=') return 1;
	else if (c == '+') return 1;
	else if (c == '-') return 1;
	else if (c == '*') return 1;
	else if (c == '%') return 1;
	else return 0;
}

void freeArray(char **toFree, int numStrings)
{
	int i;

	for (i = 0; i < numStrings; i++)
		free(toFree[i]);

	free(toFree);
}
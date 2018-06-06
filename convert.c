#include "convert.h"
#include "parser.h"

void convert(char *fileName, char **data, int numStrings)
{
	FILE *file;
	int i, numClasses, currClass, numVarFunc, maxNumPointers, maxLen;
	char *temp;
	classData *classes;

	file = fopen(fileName, "w");

	numVarFunc = getNumVarFunc(data, numStrings) + 1;
	numClasses = getNumClasses(data, numStrings);
	maxNumPointers = getNumPointers(data, numStrings);
	maxLen = getMaxLen(data, numStrings);

	temp = malloc(sizeof(char) * (maxLen + maxNumPointers + 7));

	/*Malloc everything*/
	classes = malloc(sizeof(classData) * numClasses);

	for (i = 0; i < numClasses; i++)
	{
		int j;
		classes[i].variables = malloc(sizeof(var) * numVarFunc);

		for (j = 0; j < numVarFunc; j++)
		{
			classes[i].variables[j].type = malloc(sizeof(char) * (7 + maxNumPointers));
			classes[i].variables[j].name = malloc(sizeof(char) * maxLen);
			classes[i].variables[j].value = malloc(sizeof(char) * maxLen);
		}

		classes[i].functions = malloc(sizeof(func) * numVarFunc);

		for (j = 0; j < numVarFunc; j++)
		{
			classes[i].functions[j].usesClassVar = 0;
			classes[i].functions[j].oldName = malloc(sizeof(char) * maxLen);
			classes[i].functions[j].newName = malloc(sizeof(char) * ((maxLen * 2) + numVarFunc));
			classes[i].functions[j].entireFunction = malloc(sizeof(char) * getMaxFunction(data, numStrings));
			classes[i].functions[j].param = malloc(sizeof(char) * getMaxFunction(data, numStrings));
		}
	}

	currClass = 0;

	for (i = 0; i < numStrings; i++)
	{
		if (strcmp(data[i], "class") == 0)
		{
			int currFunc, currVar, declaration;
			fprintf(file, "struct ");
			i++;

			declaration = checkDeclaration(data, i);
			if (declaration == 1)
			{
				currVar = 0;
				currFunc = 0;
				/*Deal with storing information in the class into the struct*/
				i = skipWhiteSpace(i, data);

				/*Store name of class*/
				classes[currClass].name = malloc(sizeof(char) * strlen(data[i]));
				strcpy(classes[currClass].name, data[i]);

				/*Print name of class and everything up to {*/
				while (data[i][0] != '{')
				{
					fprintf(file, "%s", data[i]);
					i++;
				}
				fprintf(file, "%s", data[i]);
				i++;

				/*Look for types then store functions or variables or print other wise*/
				while (1)
				{
					/*check termination*/
					int j = i;
					if (data[j][0] == '}')
					{
						j++;
						j = skipWhiteSpace(j, data);
						if (data[j][0] == ';')
						{
							fprintf(file, "};\n\n");

							/*Print functions of current class*/
							int k;
							for (k = 0; k < currFunc + 1; k++)
							{
								int m, once = 0;
								for (m = 0; m < strlen(classes[currClass].functions[k].entireFunction); m++)
								{
									if (classes[currClass].functions[k].usesClassVar == 1 && classes[currClass].functions[k].entireFunction[m] == '(' && once == 0)
									{
										once = 1;
										fprintf(file, "(struct %s *structPtr", classes[currClass].name);
										m++;

										while (isFormatting(classes[currClass].functions[k].entireFunction[m]))
											m++;

										if (classes[currClass].functions[k].entireFunction[m] != ')' && classes[currClass].functions[k].entireFunction[m] != ',')
											fprintf(file, ", ");
									}
									fprintf(file, "%c", classes[currClass].functions[k].entireFunction[m]);
								}
								if ((k + 1) != (currFunc + 1))
									fprintf(file, "\n\n");
							}

							/*Print constructor*/
							fprintf(file, "void constructor%s(struct %s *x)\n{\n", classes[currClass].name, classes[currClass].name);
							for (k = 0; k < currFunc; k++)
							{
								fprintf(file, "    x->%s = %s;\n", classes[currClass].functions[k].newName, classes[currClass].functions[k].newName);
							}
							fprintf(file, "}\n\n");

							/*Quit out of current class*/
							break;
						}
					}

					/*Is a type*/
					if (isType(i, data) != 0)
					{
						/*Variable*/
						if (checkDeclaration(data, i) == 0)
						{
							int print = i;
							while (data[print][0] != ';')
							{
								fprintf(file, "%s", data[print]);
								print++;
							}
							fprintf(file, ";");

							memset(temp, 0, strlen(temp));

							/*Copy type*/
							strcat(temp, data[i]);
							i++;
							/*Deal with pointers*/
							while (data[i][0] == '*' || isFormatting(data[i][0]))
							{
								if (data[i][0] == '*')
									temp[strlen(temp)] = '*';
								i++;
							}

							strcpy(classes[currClass].variables[currVar].type, temp);
							memset(temp, 0, strlen(temp));

							/*Name of var*/
							strcpy(classes[currClass].variables[currVar].name, data[i]);
							i++;

							while(data[i][0] == '=' || isFormatting(data[i][0]))
								i++;

							/*Store value, if {; ,} to say stuff about variable later*/
							strcpy(classes[currClass].variables[currVar].value, data[i]);

							/*If , deal with new variables until a ; is reached*/
							if (data[i][0] == ',')
							{
								i++;
								currVar++;
								while (data[i][0] != ';')
								{
									/*Type*/
									strcpy(classes[currClass].variables[currVar].type, classes[currClass].variables[currVar - 1].type);
									/*Name*/
									i = skipWhiteSpace(i, data);
									strcpy(classes[currClass].variables[currVar].name, data[i]);
									/*Value*/
									while (data[i][0] != ';' && data[i][0] != ',')
										i++;
									strcpy(classes[currClass].variables[currVar].value, data[i]);

									if (data[i][0] == ',')
										currVar++;
									else if (data[i][0] == ';')
										break;

									i++;
								}
							}

							i = print;
							currVar++;
						}
						/*Function*/
						else if (checkDeclaration(data, i) == 1)
						{
							int holdStart = i;

							int braces = 0;

							/*Print type*/
							fprintf(file, "%s", data[i]);
							i++;
							/*Print pointers and everything*/
							while (data[i][0] == '*' || isFormatting(data[i][0]))
							{
								fprintf(file, "%s", data[i]);
								i++;
							}

							/*Old Name*/
							strcpy(classes[currClass].functions[currFunc].oldName, data[i]);
							i++;

							/*Add class name then old name to new name*/
							strcat(classes[currClass].functions[currFunc].newName, classes[currClass].name);
							strcat(classes[currClass].functions[currFunc].newName, classes[currClass].functions[currFunc].oldName);

							/*Skip to (*/
							i = skipWhiteSpace(i, data);

							/*Parameters*/
							int exit = 0;
							while (1)
							{
								strcat(classes[currClass].functions[currFunc].param, data[i]);
								if (data[i][0] == '(')
									exit++;
								else if (data[i][0] == ')')
									exit--;
								if (data[i][0] == ')' && exit == 0)
									break;

								/*Store first letter of type for method overloading*/
								if (isType(i, data) != 0)
									classes[currClass].functions[currFunc].newName[strlen(classes[currClass].functions[currFunc].newName)] = data[i][0];
								i++;
							}

							/*Store entire function*/
							int store = holdStart;
							while (1)
							{
								int m;
								for (m = 0; m < currVar; m++)
								{
									if (braces >= 1 && strcmp(classes[currClass].variables[m].name, data[holdStart]) == 0 && checkLocalParam(store, data, classes[currClass].variables[m].name))
									{
										classes[currClass].functions[currFunc].usesClassVar = 1;
										strcat(classes[currClass].functions[currFunc].entireFunction, "structPtr->");
									}
								}

								if (data[holdStart][0] == '{')
									braces++;
								if (data[holdStart][0] == '}')
									braces--;
								if (data[holdStart][0] == '}' && braces == 0)
									break;

								if (strcmp(data[holdStart], classes[currClass].functions[currFunc].oldName) == 0)
									strcat(classes[currClass].functions[currFunc].entireFunction, classes[currClass].functions[currFunc].newName);
								else
									strcat(classes[currClass].functions[currFunc].entireFunction, data[holdStart]);
								holdStart++;
							}
							strcat(classes[currClass].functions[currFunc].entireFunction, "}");

							/*Print func pointer to struct*/
							fprintf(file, "(*%s) ();", classes[currClass].functions[currFunc].newName);
							i = holdStart;

							currFunc++;
						}
					}
					/*is a struct*/
					else if (strcmp(data[i], "struct") == 0)
					{

					}
					/*Is whitespace/formatting*/
					else
						fprintf(file, "%s", data[i]);

					i++;
				}

				/*Skip the ; after class*/
				i = skipWhiteSpace(i, data);
				i++;
			}
			else if (declaration == 0)
			{
				int iStructName, iVar;
				
				i = skipWhiteSpace(i, data);
				iStructName = i;
				i++;
				i = skipWhiteSpace(i, data);
				iVar = i;
				i++;
				i = skipWhiteSpace(i, data);

				fprintf(file, "%s %s", data[iStructName], data[iVar]);
				fprintf(file, "%s\n", data[i]);

				/*Print constructor*/
				fprintf(file, "constructor%s(&%s);", data[iStructName], data[iVar]);
			}

			if (declaration == 1)
			{
				classes[currClass].numVar = currVar;
				classes[currClass].numFunc = currFunc;
				currClass++;
			}
		}
		else
		{
			/*Replace new function names*/
			int l, m, storel, storem, printNew = 0;
			for (l = 0; l < currClass; l++)
			{
				for (m = 0; m < classes[l].numFunc; m++)
				{
					if (strcmp(data[i], classes[l].functions[m].oldName) == 0)
					{
						storel = l;
						storem = m;
						printNew = 1;
					}
				}
			}

			if (printNew == 1)
			{
				if (classes[storel].functions[storem].usesClassVar == 1)
				{
					int q = i - 1;
					while (data[q][0] == '.' || isFormatting(data[q][0]))
						q--;
					
					fprintf(file, "%s", classes[storel].functions[storem].newName);
					
					while (data[i][0] != '(')
						i++;

					fprintf(file, "%s", data[i]);
					i++;

					fprintf(file, "&%s", data[q]);
					i = skipWhiteSpace(i, data);
					if (data[i][0] != ')')
					{
						fprintf(file, ", ");
						fprintf(file, "%s", data[i]);
					}
					else
						fprintf(file, ")");
				}
				else
					fprintf(file, "%s", classes[storel].functions[storem].newName);
			}
			else
				fprintf(file, "%s", data[i]);
		}
	}

	for (i = 0; i < numClasses; i++)
	{
		int j;
		/*Free variables*/
		for (j = 0; j < numVarFunc; j++)
		{
			free(classes[i].variables[j].type);
			free(classes[i].variables[j].name);
			free(classes[i].variables[j].value);
		}
		free(classes[i].variables);

		/*Free functions*/
		for (j = 0; j < numVarFunc; j++)
		{
			free(classes[i].functions[j].oldName);
			free(classes[i].functions[j].newName);
			free(classes[i].functions[j].entireFunction);
			free(classes[i].functions[j].param);
		}
		free(classes[i].functions);

		free(classes[i].name);
	}

	free(temp);
	free(classes);
	fclose(file);
}

char* changeExtension(char *fileName)
{
	char *newFileName;

	newFileName = malloc(sizeof(char) * strlen(fileName));

	strcpy(newFileName, fileName);
	newFileName[strlen(newFileName) - 1] = 0;

	return newFileName;
}

int getNumClasses(char **data, int numStr)
{
	int numClasses = 0, i;

	for (i = 0; i < numStr; i++)
	{
		if (strcmp(data[i], "class") == 0 && checkDeclaration(data, i) == 1)
			numClasses++;
	}

	return numClasses;
}

int checkDeclaration(char **data, int currPos)
{
	int j;

	j = currPos;

	while (data[j][0] != ';' && data[j][0] != '{')
		j++;

	if (data[j][0] == ';')
		return 0;
	else if (data[j][0] == '{')
		return 1;
	else
		return -1;
}

int isType(int i, char **data)
{
	if (strcmp(data[i], "int") == 0) return 3;
	else if (strcmp(data[i], "float") == 0) return 5;
	else if (strcmp(data[i], "char") == 0) return 4;
	else if (strcmp(data[i], "double") == 0) return 6;
	else if (strcmp(data[i], "long") == 0) return 4;
	else if (strcmp(data[i], "short") == 0) return 5;
	else if (strcmp(data[i], "void") == 0) return 4;
	else return 0;
}

int skipWhiteSpace(int i, char **data)
{
	int iToReturn;
	iToReturn = i;

	while (data[iToReturn][0] == ' ' || data[iToReturn][0] == '\t' || data[iToReturn][0] == '\n' || data[iToReturn][0] == '\r' || data[iToReturn][0] == '\0')
		iToReturn++;

	return iToReturn;
}

int getNumVarFunc(char **data, int numStr)
{
	int i, max = 0, curr;

	for (i = 0; i < numStr; i++)
	{
		if (strcmp(data[i], "class") == 0 && checkDeclaration(data, i))
		{
			int j = i + 1;
			curr = 0;

			while (1)
			{
				if (j == numStr)
					break;
				/*Check for end of class*/
				if (data[j][0] == '}')
				{
					int k = j;
					if (data[k + 1] != NULL)
						k++;
					k = skipWhiteSpace(k, data);
					if (data[k][0] == ';')
						break;
				}

				if (isType(j, data) != 0 || data[j][0] == ',')
					curr++;
				j++;
			}
			i = j;

			if (curr > max)
				max = curr;
		}
	}

	return max;
}

int getNumPointers(char **data, int numStr)
{
	int numPtrs = 0, i, j;

	for (i = 0; i < numStr; i++)
	{
		for (j = 0; j < strlen(data[i]); j++)
		{
			if (data[i][j] == '*')
			{
				numPtrs++;
			}
		}
	}
	return numPtrs;
}

int getMaxLen(char **data, int numStr)
{
	int maxLen = 0, i;

	for (i = 0; i < numStr; i++)
		if (strlen(data[i]) > maxLen)
			maxLen = strlen(data[i]);

	return maxLen;
}

int getMaxFunction(char **data, int numStr)
{
	int maxFunc = 0, curr, i, numOpen = 0;

	for (i = 0; i < numStr; i++)
	{
		if (isType(i, data) != 0 && checkDeclaration(data, i) == 1)
		{
			curr = 0;
			while (1)
			{
				if (data[i][0] == '{')
					numOpen++;
				if (data[i][0] == '}')
					numOpen--;
				if (data[i][0] == '}' && numOpen == 0)
					break;
				curr += strlen(data[i]) + 1;
				i++;
			}
			if (curr > maxFunc)
				maxFunc = curr;
		}
	}
	return maxFunc;
}

int checkLocalParam(int funcPos, char **data, char *varName)
{
	int i = funcPos, numBrace = 0;
	while (1)
	{
		if (data[i][0] == '{')
			numBrace++;
		if (data[i][0] == '}')
			numBrace--;
		if (data[i][0] == '}' && numBrace == 0)
			break;

		if (isType(i, data) != 0)
		{
			i++;
			i = skipWhiteSpace(i, data);
			if (strcmp(varName, data[i]) == 0)
				return 0;
		}
		i++;
	}
	return 1;
}
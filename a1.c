#include "parser.h"
#include "convert.h"

int main(int argc, char** argv)
{
	char* fileName;
	char** data = NULL;
	int numStrings;

	fileName = argv[1];

	if (argc > 1)
	{
		if (fileName[strlen(fileName) - 3] != '.' || fileName[strlen(fileName) - 2] != 'c' || fileName[strlen(fileName) - 1] != 'c')
		{
			printf("Make sure input file has extension \".cc\".\n");
			printf("Program will now exit.\n");
			exit(0);
		}
	}

	data = parseFile(fileName, &numStrings);

	fileName = changeExtension(fileName);

	convert(fileName, data, numStrings);

	freeArray(data, numStrings);
	free(fileName);
	return 0;
}
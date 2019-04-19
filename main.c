#include <stdio.h>
#include <stdlib.h>

#define MAX_CODE_STRING_LENGTH 300000
#define TAPE_LENGTH 300000

char codeString[MAX_CODE_STRING_LENGTH] = {0},
	 tape[TAPE_LENGTH] = {0};

char *tapePointer = tape,
	 *codePointer = codeString;

char *forwardJumpsDestinations[MAX_CODE_STRING_LENGTH] = {0},
	 *backwardJumpsDestinations[MAX_CODE_STRING_LENGTH] = {0};

void (*functions[94])() = {};

void exitProgram()
{ exit(0); }

void moveRight()
{ ++tapePointer;  ++codePointer; }

void moveLeft()
{ --tapePointer; ++codePointer; }

void increment()
{ ++*tapePointer; ++codePointer; }

void decrement()
{ --*tapePointer; ++codePointer; }

void outputChar()
{ putc(*tapePointer, stdout); ++codePointer; }

void inputChar()
{ fflush(stdin); *tapePointer = getchar(); ++codePointer; }

int bracketsLevel = 0;

void jumpForward()
{
	if (*tapePointer)
	{
		++codePointer;
		return;
	}
	unsigned int codePointerIndex = codePointer - codeString;
	if (!forwardJumpsDestinations[codePointerIndex])
	{
		++bracketsLevel;
		do
		{
			++codePointer;
			if (*codePointer == ']')
				--bracketsLevel;
			else
			if (*codePointer == '[')
				++bracketsLevel;
		} while (bracketsLevel);
		++codePointer;
		forwardJumpsDestinations[codePointerIndex] = codePointer;
	}
	else
	{
		//printf("used remembered destination!\n");
		codePointer = forwardJumpsDestinations[codePointerIndex];
	}
}

void jumpBackward()
{
	if (!*tapePointer)
	{
		++codePointer;
		return;
	}
	unsigned int codePointerIndex = codePointer - codeString;
	if (!backwardJumpsDestinations[codePointerIndex])
	{
		--bracketsLevel;
		do
		{
			--codePointer;
			if (*codePointer == ']')
				--bracketsLevel;
			else
			if (*codePointer == '[')
				++bracketsLevel;
		} while (bracketsLevel);
		++codePointer;
		backwardJumpsDestinations[codePointerIndex] = codePointer;
	}
	else
	{
		//printf("used remembered destination!\n");
		codePointer = backwardJumpsDestinations[codePointerIndex];
	}
}

char *createFilter(const char *allowedChars)
{
	char **filterPointer = (char**)malloc(sizeof(char*));
	*filterPointer = (char*)calloc(127, sizeof(char));
	char *filter = *filterPointer;
	for (; *allowedChars; allowedChars++)
		filter[*allowedChars] = 1;
	return *filterPointer;
}

int main(void)
{
	char fileName[256];
	printf("File name: ");
	scanf("%256s", fileName);
	FILE *file = fopen(fileName, "rb");
	if (!file)
	{
		printf("Error while opening file\n");
		return 1;
	}
	char *filter = createFilter("><+-.,[]");
	while (1)
	{
		*codePointer = (char)getc(file);
		if (*codePointer == '0')
			break;
		if (filter[*codePointer])
		{
			//printf("%c", *codePointer);
			++codePointer;
		}
	}
	*codePointer = 0;
	codePointer = codeString;

	functions[0] = exitProgram;
	functions['>'] = moveRight;
	functions['<'] = moveLeft;
	functions['+'] = increment;
	functions['-'] = decrement;
	functions['.'] = outputChar;
	functions[','] = inputChar;
	functions['['] = jumpForward;
	functions[']'] = jumpBackward;

	while (1)
	{
		//printf("%d -> %c\n", codePointer - codeString, *codePointer);
		functions[*codePointer]();
	}

	return 0;
}

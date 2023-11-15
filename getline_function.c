#include "shell.h"

/**
* readCommandLine - Read one line from the prompt.
* @progData: Struct for the program's data
*
* Return: Number of bytes read.
*/
int readCommandLine(ProgramData *progData)
{
	char inputBuffer[BUFFER_SIZE] = {'\0'};
	static char *commandsArray[10] = {NULL};
	static char operatorsArray[10] = {'\0'};
	ssize_t bytesRead;
	size_t k = 0;

	/* Check if there are no more commands in the array */
	/* and check the logical operators */
	if (!commandsArray[0] || (operatorsArray[0] == '&' && errno != 0) ||
		(operatorsArray[0] == '|' && errno == 0))
	{
		/* Free the memory allocated in the array if it exists */
		for (k = 0; commandsArray[k]; k++)
		{
			free(commandsArray[k]);
			commandsArray[k] = NULL;
		}

		/* Read from the file descriptor into inputBuffer */
		bytesRead = read(progData->fileDescriptor, &inputBuffer, BUFFER_SIZE - 1);
		if (bytesRead == 0)
			return (-1);

		/* Split lines for '\n' or ';' */
		k = 0;
		do {
			commandsArray[k] = strDuplicate(_strtok(k ? NULL : inputBuffer, "\n;"));
			/* Checks and split for && and || operators */
			k = checkLogicOperators(commandsArray, k, operatorsArray);
		} while (commandsArray[k++]);
	}

	/* Obtain the next command (command 0) and remove it from the array */
	progData->inputLine = commandsArray[0];
	for (k = 0; commandsArray[k]; k++)
	{
		commandsArray[k] = commandsArray[k + 1];
		operatorsArray[k] = operatorsArray[k + 1];
	}

	return (strLength(progData->inputLine));
}

/**
* checkLogicOperators - Checks and split for && and || operators
* @commandsArray: Array of the commands.
* @index: Index in the commandsArray to be checked
* @operatorsArray: Array of the logical operators for each previous command
*
* Return: Index of the last command in the commandsArray.
*/
int checkLogicOperators(char *commandsArray[], int index, char operatorsArray[])
{
	char *temp = NULL;
	int l;

	/* Checks for the '&' char in the command line */
	for (l = 0; commandsArray[index] != NULL && commandsArray[index][l]; l++)
	{
		if (commandsArray[index][l] == '&' && commandsArray[index][l + 1] == '&')
		{
			/* Split the line when chars && are found */
			temp = commandsArray[index];
			commandsArray[index][l] = '\0';
			commandsArray[index] = strDuplicate(commandsArray[index]);
			commandsArray[index + 1] = strDuplicate(temp + l + 2);
			index++;
			operatorsArray[index] = '&';
			free(temp);
			l = 0;
		}
		if (commandsArray[index][l] == '|' && commandsArray[index][l + 1] == '|')
		{
			/* Split the line when chars || are found */
			temp = commandsArray[index];
			commandsArray[index][l] = '\0';
			commandsArray[index] = strDuplicate(commandsArray[index]);
			commandsArray[index + 1] = strDuplicate(temp + l + 2);
			index++;
			operatorsArray[index] = '|';
			free(temp);
			l = 0;
		}
	}
	return index;
}

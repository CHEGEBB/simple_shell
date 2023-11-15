#include "shell.h"

/**
* readCommandLine - read one line from the prompt.
* @progData: struct for the program's data
*
* Return: number of bytes read.
*/
int readCommandLine(ProgramData *progData)
{
    char inputBuffer[BUFFER_SIZE] = {'\0'};
    static char *commandsArray[10] = {NULL};
    static char operatorsArray[10] = {'\0'};
    ssize_t bytesRead;
    size_t i = 0;

    /* Check if there are no more commands in the array */
    /* and check the logical operators */
    if (!commandsArray[0] || (operatorsArray[0] == '&' && errno != 0) ||
        (operatorsArray[0] == '|' && errno == 0))
    {
        /* Free the memory allocated in the array if it exists */
        for (i = 0; commandsArray[i]; i++)
        {
            free(commandsArray[i]);
            commandsArray[i] = NULL;
        }

        /* Read from the file descriptor into inputBuffer */
        bytesRead = read(progData->fileDescriptor, &inputBuffer, BUFFER_SIZE - 1);
        if (bytesRead == 0)
            return (-1);

        /* Split lines for '\n' or ';' */
        i = 0;
        do {
            commandsArray[i] = strDuplicate(_strtok(i ? NULL : inputBuffer, "\n;"));
            /* Checks and split for && and || operators */
            i = checkLogicOperators(commandsArray, i, operatorsArray);
        } while (commandsArray[i++]);
    }

    /* Obtain the next command (command 0) and remove it from the array */
    progData->inputLine = commandsArray[0];
    for (i = 0; commandsArray[i]; i++)
    {
        commandsArray[i] = commandsArray[i + 1];
        operatorsArray[i] = operatorsArray[i + 1];
    }

    return (strLength(progData->inputLine));
}

/**
* checkLogicOperators - checks and split for && and || operators
* @commandsArray: array of the commands.
* @index: index in the commandsArray to be checked
* @operatorsArray: array of the logical operators for each previous command
*
* Return: index of the last command in the commandsArray.
*/
int checkLogicOperators(char *commandsArray[], int index, char operatorsArray[])
{
    char *temp = NULL;
    int j;

    /* Checks for the '&' char in the command line */
    for (j = 0; commandsArray[index] != NULL && commandsArray[index][j]; j++)
    {
        if (commandsArray[index][j] == '&' && commandsArray[index][j + 1] == '&')
        {
            /* Split the line when chars && are found */
            temp = commandsArray[index];
            commandsArray[index][j] = '\0';
            commandsArray[index] = strDuplicate(commandsArray[index]);
            commandsArray[index + 1] = strDuplicate(temp + j + 2);
            index++;
            operatorsArray[index] = '&';
            free(temp);
            j = 0;
        }
        if (commandsArray[index][j] == '|' && commandsArray[index][j + 1] == '|')
        {
            /* Split the line when chars || are found */
            temp = commandsArray[index];
            commandsArray[index][j] = '\0';
            commandsArray[index] = strDuplicate(commandsArray[index]);
            commandsArray[index + 1] = strDuplicate(temp + j + 2);
            index++;
            operatorsArray[index] = '|';
            free(temp);
            j = 0;
        }
    }
    return index;
}

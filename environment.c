#include "shell.h"

/**
 * fetchEnvironmentVariable - Retrieve the value of an environment variable.
 * @varName: The name of the environment variable to fetch.
 * @programData: Structure holding the program's data.
 * Return: A pointer to the variable's value or NULL if not found.
 */
char *fetchEnvironmentVariable(char *varName, data_of_program *programData)
{
    int index, varLength = 0;

    /* Verify arguments */
    if (varName == NULL || programData->env == NULL)
        return NULL;

    /* Determine the length of the requested variable */
    varLength = strLength(varName);

    for (index = 0; programData->env[index]; index++)
    {
        /* Iterate through the environment, searching for the specified variable */
        if (strCompare(varName, programData->env[index], varLength) &&
            programData->env[index][varLength] == '=')
        {
            /* Return the value of the variable (NAME=) */
            return (programData->env[index] + varLength + 1);
        }
    }
    /* Return NULL if the variable is not found */
    return NULL;
}

/**
 * modifyEnvironmentVariable - Overwrite or create an environment variable.
 * @varName: The name of the variable to modify.
 * @value: The new value for the variable.
 * @programData: Structure holding the program's data.
 * Return: 1 if parameters are NULL, 2 if an error occurs, 0 if successful.
 */
int modifyEnvironmentVariable(char *varName, char *value, data_of_program *programData)
{
    int index, varLength = 0, isNewVar = 1;

    /* Verify arguments */
    if (varName == NULL || value == NULL || programData->env == NULL)
        return 1;

    /* Determine the length of the specified variable */
    varLength = strLength(varName);

    for (index = 0; programData->env[index]; index++)
    {
        /* Iterate through the environment, checking for the variable's existence */
        if (strCompare(varName, programData->env[index], varLength) &&
            programData->env[index][varLength] == '=')
        {
            /* If the variable exists, free its memory as it will be recreated */
            isNewVar = 0;
            free(programData->env[index]);
            break;
        }
    }

    /* Create a string in the form key=value */
    programData->env[index] = strConcat(stringDuplicate(varName), "=");
    programData->env[index] = strConcat(programData->env[index], value);

    if (isNewVar)
    {
        /* If the variable is new, place it at the end of the list and set NULL in the next position */
        programData->env[index + 1] = NULL;
    }

    return 0;
}

/**
 * eliminateEnvironmentVariable - Remove a variable from the environment.
 * @varName: The name of the variable to remove.
 * @programData: Structure holding the program's data.
 * Return: 1 if the variable was removed, 0 if the variable does not exist.
 */
int eliminateEnvironmentVariable(char *varName, data_of_program *programData)
{
    int index, varLength = 0;

    /* Verify arguments */
    if (varName == NULL || programData->env == NULL)
        return 0;

    /* Determine the length of the specified variable */
    varLength = strLength(varName);

    for (index = 0; programData->env[index]; index++)
    {
        /* Iterate through the environment, checking for coincidences */
        if (strCompare(varName, programData->env[index], varLength) &&
            programData->env[index][varLength] == '=')
        {
            /* If the variable exists, remove it */
            free(programData->env[index]);

            /* Move the other variables one position down */
            index++;
            for (; programData->env[index]; index++)
            {
                programData->env[index - 1] = programData->env[index];
            }

            /* Set NULL at the new end of the list */
            programData->env[index - 1] = NULL;
            return 1;
        }
    }
    return 0;
}

/**
 * displayCurrentEnvironment - Print the current environment.
 * @programData: Structure holding the program's data.
 * Return: Nothing.
 */
void displayCurrentEnvironment(data_of_program *programData)
{
    int j;

    for (j = 0; programData->env[j]; j++)
    {
        _print(programData->env[j]);
        _print("\n");
    }
}

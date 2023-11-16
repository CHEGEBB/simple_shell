#include "shell.h"

/**
 * executeEnvCommand - Display the current environment.
 * @programData: Struct holding program data.
 * Return: 0 if successful, or an error code if specified in arguments.
 */
int executeEnvCommand(ProgramData *programData)
{
    int index;
    char varName[50] = {'\0'};
    char *varCopy = NULL;

    /* If no arguments, display the entire environment. */
    if (programData->commandArguments[1] == NULL)
        displayEnvironment(programData);
    else
    {
        /* Iterate through the provided argument to check for '=' character. */
        for (index = 0; programData->commandArguments[1][index]; index++)
        {
            if (programData->commandArguments[1][index] == '=')
            {
                /* Check if a variable with the same name exists and temporarily change its value. */
                varCopy = duplicateEnvironmentVariable(varName, programData);
                if (varCopy != NULL)
                    setEnvironmentVariable(varName, programData->commandArguments[1] + index + 1, programData);

                /* Display the modified environment. */
                displayEnvironment(programData);

                if (duplicateEnvironmentVariable(varName, programData) == NULL)
                {
                    /* Display the variable if it does not exist in the environment. */
                    printString(programData->commandArguments[1]);
                    printString("\n");
                }
                else
                {
                    /* Restore the original value of the variable. */
                    setEnvironmentVariable(varName, varCopy, programData);
                    free(varCopy);
                }
                return 0;
            }
            varName[index] = programData->commandArguments[1][index];
        }
        /* Display an error if '=' is not found in the provided argument. */
        setErrorAndPrint(programData->commandName);
    }
    return 0;
}

/**
 * executeSetEnvCommand - Set or update an environment variable.
 * @programData: Struct holding program data.
 * Return: 0 if successful, or an error code if specified in arguments.
 */
int executeSetEnvCommand(ProgramData *programData)
{
    /* Validate the arguments. */
    if (programData->commandArguments[1] == NULL || programData->commandArguments[2] == NULL)
        return 0;

    /* Check for an excessive number of arguments. */
    if (programData->commandArguments[3] != NULL)
    {
        setErrorAndPrint(programData->commandName);
        return 5;
    }

    /* Set or update the environment variable. */
    setEnvironmentVariable(programData->commandArguments[1], programData->commandArguments[2], programData);

    return 0;
}

/**
 * executeUnsetEnvCommand - Remove an environment variable.
 * @programData: Struct holding program data.
 * Return: 0 if successful, or an error code if specified in arguments.
 */
int executeUnsetEnvCommand(ProgramData *programData)
{
    /* Validate the arguments. */
    if (programData->commandArguments[1] == NULL)
        return 0;

    /* Check for an excessive number of arguments. */
    if (programData->commandArguments[2] != NULL)
    {
        setErrorAndPrint(programData->commandName);
        return 5;
    }

    /* Remove the environment variable. */
    removeEnvironmentVariable(programData->commandArguments[1], programData);

    return 0;
}

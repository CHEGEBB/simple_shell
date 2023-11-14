#include "shell.h"

/**
 * performExit - Exit the program with the specified status.
 * @programData: Pointer to the program's data.
 * Return: Zero on success, or a specific number if declared in the arguments.
 */
int performExit(data_of_program *programData)
{
    int index;

    if (programData->tokens[1] != NULL)
    {
        /* Check if the argument for exit is a number */
        for (index = 0; programData->tokens[1][index]; index++)
        {
            if ((programData->tokens[1][index] < '0' || programData->tokens[1][index] > '9') &&
                programData->tokens[1][index] != '+')
            {
                /* Not a number, return error */
                errno = 2;
                return 2;
            }
        }
        errno = _atoi(programData->tokens[1]);
    }
    freeAllData(programData);
    exit(errno);
}

/**
 * changeDirectory - Change the current directory.
 * @programData: Pointer to the program's data.
 * Return: Zero on success, or a specific number if declared in the arguments.
 */
int changeDirectory(data_of_program *programData)
{
    char *homeDir = envGetKey("HOME", programData), *oldDir = NULL;
    char previousDir[128] = {0};
    int errorCode = 0;

    if (programData->tokens[1])
    {
        if (strCompare(programData->tokens[1], "-", 0))
        {
            /* Change to the previous directory */
            oldDir = envGetKey("OLDPWD", programData);
            if (oldDir)
                errorCode = setWorkingDirectory(programData, oldDir);
            _print(envGetKey("PWD", programData));
            _print("\n");

            return errorCode;
        }
        else
        {
            return setWorkingDirectory(programData, programData->tokens[1]);
        }
    }
    else
    {
        if (!homeDir)
            homeDir = getcwd(previousDir, 128);

        return setWorkingDirectory(programData, homeDir);
    }
    return 0;
}

/**
 * setWorkingDirectory - Set the working directory.
 * @programData: Pointer to the program's data.
 * @newDir: Path to be set as the working directory.
 * Return: Zero on success, or a specific number if declared in the arguments.
 */
int setWorkingDirectory(data_of_program *programData, char *newDir)
{
    char previousDir[128] = {0};
    int errorCode = 0;

    getcwd(previousDir, 128);

    if (!strCompare(previousDir, newDir, 0))
    {
        errorCode = chdir(newDir);
        if (errorCode == -1)
        {
            errno = 2;
            return 3;
        }
        envSetKey("PWD", newDir, programData);
    }
    envSetKey("OLDPWD", previousDir, programData);
    return 0;
}

/**
 * provideHelp - Display help information about the shell.
 * @programData: Pointer to the program's data.
 * Return: Zero on success, or a specific number if declared in the arguments.
 */
int provideHelp(data_of_program *programData)
{
    int i, length = 0;
    char *messages[6] = {NULL};

    messages[0] = HELP_MESSAGE;

    /* Validate arguments */
    if (programData->tokens[1] == NULL)
    {
        _print(messages[0] + 6);
        return 1;
    }
    if (programData->tokens[2] != NULL)
    {
        errno = E2BIG;
        perror(programData->command_name);
        return 5;
    }
    messages[1] = HELP_EXIT_MESSAGE;
    messages[2] = HELP_ENV_MESSAGE;
    messages[3] = HELP_SETENV_MESSAGE;
    messages[4] = HELP_UNSETENV_MESSAGE;
    messages[5] = HELP_CD_MESSAGE;

    for (i = 0; messages[i]; i++)
    {
        length = strLength(programData->tokens[1]);
        if (strCompare(programData->tokens[1], messages[i], length))
        {
            _print(messages[i] + length + 1);
            return 1;
        }
    }
    /* No match found, print error and return -1 */
    errno = EINVAL;
    perror(programData->command_name);
    return 0;
}

/**
 * manageAlias - Add, remove, or show aliases.
 * @programData: Pointer to the program's data.
 * Return: Zero on success, or a specific number if declared in the arguments.
 */
int manageAlias(data_of_program *programData)
{
    int index = 0;

    /* If there are no arguments, print all aliases */
    if (programData->tokens[1] == NULL)
        return print

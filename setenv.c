#include "shell.h"

/**
 * setenv_builtin - Initialize a new environment variable or modify an existing one
 * @data: struct for the program's data
 * @args: array of command arguments
 * Return: 0 on success, -1 on failure
 */
int setenv_builtin(data_of_program *data, char **args)
{
    /* Check the number of arguments */
    if (args[1] == NULL || args[2] == NULL || args[3] != NULL)
    {
        fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
        return -1;
    }

    /* Set the environment variable */
    if (setenv(args[1], args[2], 1) != 0)
    {
        perror("setenv");
        return -1;
    }

    return 0;
}

/**
 * unsetenv_builtin - Remove an environment variable
 * @data: struct for the program's data
 * @args: array of command arguments
 * Return: 0 on success, -1 on failure
 */
int unsetenv_builtin(data_of_program *data, char **args)
{
    /* Check the number of arguments */
    if (args[1] == NULL || args[2] != NULL)
    {
        fprintf(stderr, "Usage: unsetenv VARIABLE\n");
        return -1;
    }

    /* Unset the environment variable */
    if (unsetenv(args[1]) != 0)
    {
        perror("unsetenv");
        return -1;
    }

    return 0;
}

#include "shell.h"

/**
 * executeBuiltins - Search for a match and execute the associated builtin function.
 * @programData: Pointer to the program's data.
 * Return: Returns the return value of the executed function if there is a match,
 * otherwise returns -1.
 **/
int executeBuiltins(data_of_program *programData)
{
    int index;
    builtins options[] = {
        {"exit", performExit},
        {"help", provideHelp},
        {"cd", changeDirectory},
        {"alias", manageAlias},
        {"env", builtinEnv},
        {"setenv", setEnvironmentVariable},
        {"unsetenv", unsetEnvironmentVariable},
        {NULL, NULL}
    };

    /* Walk through the structure */
    for (index = 0; options[index].builtin != NULL; index++)
    {
        /* If there is a match between the given command and a builtin, */
        if (strCompare(options[index].builtin, programData->command_name, 0))
        {
            /* Execute the function and return its return value */
            return options[index].function(programData);
        }
    }
    /* If there is no match, return -1 */
    return -1;
}

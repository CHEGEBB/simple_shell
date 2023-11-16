#include "shell.h"

/**
 * Searches for a matching builtin command and executes the corresponding function.
 *
 * @param programData The program's data structure.
 * @return The return value of the executed function, or -1 if no matching builtin is found.
 */
int searchAndExecuteBuiltinCommand(dataOfProgram *programData) {
    int index;
    struct builtinOptions {
        const char *builtinName;
        int (*builtinFunction)(dataOfProgram *);
    } builtins[] = {
        {"exit", builtinExit},
        {"help", builtinHelp},
        {"cd", builtinCd},
        {"alias", builtinAlias},
        {"env", builtinEnv},
        {"setenv", builtinSetEnv},
        {"unsetenv", builtinUnsetEnv},
        {NULL, NULL}
    };

    /* Iterate through the list of builtins */
    for (index = 0; builtins[index].builtinName; ++index) {
        /* Check if the input command matches a builtin */
        if (strCompare(builtins[index].builtinName, programData->commandName, 0)) {
            /* Execute the corresponding builtin function and return its result */
            return builtins[index].builtinFunction(programData);
        }
    }

    /* No matching builtin found */
    return -1;
}

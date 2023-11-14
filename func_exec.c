#include "shell.h"

/**
 * runCommand - Execute a command with its full path variables.
 * @programData: Pointer to the program's data.
 * Return: Zero on success, -1 on failure.
 */
int runCommand(data_of_program *programData)
{
    int result = 0, exitStatus;
    pid_t childPID;

    /* Check if the command is a built-in */
    result = executeBuiltins(programData);
    if (result != -1) /* Command found among built-ins */
        return result;

    /* Search for the command in the file system */
    result = findExecutable(programData);
    if (result)
    {
        /* Command not found */
        return result;
    }
    else
    {
        /* Command found, create a child process to execute it */
        childPID = fork();
        if (childPID == -1)
        {
            /* Fork call failed */
            perror(programData->command_name);
            exit(EXIT_FAILURE);
        }
        if (childPID == 0)
        {
            /* Child process executes the command */
            result = execve(programData->tokens[0], programData->tokens, programData->env);
            if (result == -1)
            {
                /* Error occurred during execve */
                perror(programData->command_name);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            /* Parent process waits and checks the exit status of the child */
            wait(&exitStatus);
            if (WIFEXITED(exitStatus))
                errno = WEXITSTATUS(exitStatus);
            else if (WIFSIGNALED(exitStatus))
                errno = 128 + WTERMSIG(exitStatus);
        }
    }
    return 0;
}

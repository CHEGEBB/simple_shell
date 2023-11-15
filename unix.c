#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define MAX_COMMAND_LENGTH 100

/**
 * handle_ctrl_c - Signal handler for SIGINT (Ctrl+C)
 * @signo: Signal number
 */
void handle_ctrl_c(int signo)
{
    // Handle Ctrl+C
    if (signo == SIGINT)
    {
        write(STDOUT_FILENO, "\n#cisfun$", 10);
    }
}

/**
 * main - Entry point for the simple shell
 * Return: Always 0
 */
int main(void)
{
    char command[MAX_COMMAND_LENGTH];

    // Set up Ctrl+C signal handler
    signal(SIGINT, handle_ctrl_c);

    while (1)
    {
        write(STDOUT_FILENO, "#cisfun$", 10);

        // Read user input
        if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL)
        {
            // Handle end of file (Ctrl+D)
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        // Remove the newline character at the end
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n')
        {
            command[len - 1] = '\0';
        }

        // Execute the command
        if (access(command, X_OK) == 0)
        {
            pid_t pid = fork();
            if (pid == 0)
            {
                // Child process
                execl(command, command, (char *)NULL);
                // If execl fails
                perror("./shell");
                exit(EXIT_FAILURE);
            }
            else if (pid > 0)
            {
                // Parent process
                wait(NULL);
            }
            else
            {
                perror("fork");
            }
        }
        else
        {
            // If the executable is not found
            perror("./shell");
        }
    }

    return 0;
}

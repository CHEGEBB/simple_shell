#include "shell.h"

/**
 * execute: Executes a command with the given path or from the built-in commands.
 *
 * @param data: The program data structure.
 * @return: Zero if successful, or an error code otherwise.
 */
int execute(data_of_program *data) {
    int result = 0, status;
    pid_t child_pid;

    // Check if the command is a built-in function.
    result = builtins_list(data);
    if (result != -1) {
        // Execute the built-in command.
        return result;
    }

    // Search for the program in the file system.
    result = find_program(data);
    if (result) {
        // Program not found.
        return result;
    }

    // Create a child process.
    child_pid = fork();
    if (child_pid == -1) {
        // Fork failed.
        perror(data->command_name);
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Child process: execute the program.
        result = execve(data->tokens[0], data->tokens, data->env);
        if (result == -1) {
            // Execution failed.
            perror(data->command_name);
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process: wait for child to finish and check its exit status.
        wait(&status);

        if (WIFEXITED(status)) {
            // Child exited normally.
            errno = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            // Child terminated by signal.
            errno = 128 + WTERMSIG(status);
        }
    }

    return 0;
}

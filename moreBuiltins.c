#include "shell.h"

/**
 * builtin_exit: Exits the shell with the given status code.
 *
 * @param data: The program data structure.
 * @return: Zero if successful, or the given status code otherwise.
 */
int builtin_exit(data_of_program *data) {
    int status = 0;

    if (data->tokens[1] != NULL) {
        // Check if the argument is a valid number.
        for (int i = 0; data->tokens[1][i]; i++) {
            if ((data->tokens[1][i] < '0' || data->tokens[1][i] > '9') &&
                data->tokens[1][i] != '+') {
                // Not a valid number.
                errno = 2;
                return 2;
            }
        }

        // Convert the argument to an integer.
        status = _atoi(data->tokens[1]);
    }

    // Free the allocated memory.
    free_all_data(data);

    // Exit the shell with the given status code.
    exit(status);
}

/**
 * builtin_cd: Changes the current working directory.
 *
 * @param data: The program data structure.
 * @return: Zero if successful, or an error code otherwise.
 */
int builtin_cd(data_of_program *data) {
    char *home_dir = env_get_key("HOME", data), *old_dir = NULL;
    char old_pwd[128] = {0};
    int error_code = 0;

    if (data->tokens[1]) {
        if (str_compare(data->tokens[1], "-", 0)) {
            // Change to the previous working directory.
            old_dir = env_get_key("OLDPWD", data);
            if (old_dir) {
                error_code = set_work_directory(data, old_dir);
            }

            if (error_code == 0) {
                // Print the new current working directory.
                _print(env_get_key("PWD", data));
                _print("\n");
            } else {
                // Error message if changing directories failed.
                _print("Failed to change to previous working directory.\n");
            }

            return error_code;
        } else {
            // Change to the specified directory.
            return set_work_directory(data, data->tokens[1]);
        }
    } else {
        // Change to the home directory if no argument is provided.
        if (!home_dir) {
            home_dir = getcwd(old_pwd, 128);
        }

        return set_work_directory(data, home_dir);
    }
}

/**
 * set_work_directory: Sets the current working directory to the specified path.
 *
 * @param data: The program data structure.
 * @param new_dir: The new working directory path.
 * @return: Zero if successful, or an error code otherwise.
 */
int set_work_directory(data_of_program *data, char *new_dir) {
    char old_dir[128] = {0};
    int error_code = 0;

    // Get the current working directory.
    getcwd(old_dir, 128);

    // Check if the new directory is different from the current one.
    if (!str_compare(old_dir, new_dir, 0)) {
        // Change the working directory.
        error_code = chdir(new_dir);
        if (error_code == -1) {
            // Error message if changing directories failed.
            _print("Failed to change directory.\n");

            errno = 2;
            return 3;
        }

        // Update the PWD environment variable.
        env_set_key("PWD", new_dir, data);
    }

    // Update the OLDPWD environment variable.
    env_set_key("OLDPWD", old_dir, data);
    return 0;
}

/**
 * builtin_help: Provides help information for shell commands.
 *
 *
/

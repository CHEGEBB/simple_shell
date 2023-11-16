#include "shell.h"

/*
 * check_file - Check if a file exists and is executable.
 * @full_path: The full path to the file.
 * Return: 0 on success, errno otherwise.
 */
int check_file(char *full_path) {
    struct stat file_stat;

    if (stat(full_path, &file_stat) == -1) {
        /* File does not exist */
        errno = ENOENT;
        return errno;
    }

    if (S_ISDIR(file_stat.st_mode)) {
        /* File is a directory */
        errno = EISDIR;
        return errno;
    }

    if (access(full_path, X_OK) != 0) {
        /* File lacks execute permissions */
        errno = EACCES;
        return errno;
    }

    return 0; /* File exists, is not a directory, and has execute permissions */
}

/*
 * find_program - Search the PATH for the specified program.
 * @data: Pointer to the program data structure.
 * Return: 0 on success, errno otherwise.
 */
int find_program(program_data *data) {
    int index = 0;
    int result;
    char **directories;

    if (!data->command_name) {
        return EINVAL; /* Invalid command name */
    }

    /* Check if the command is a full path or an executable in the current directory */
    if (data->command_name[0] == '/' || data->command_name[0] == '.') {
        return check_file(data->command_name);
    }

    /* Free the original command name token */
    free(data->tokens[0]);

    /* Prepend the command name with "/" to form a full path */
    data->tokens[0] = str_concat(str_duplicate("/"), data->command_name);
    if (!data->tokens[0]) {
        return ENOMEM; /* Memory allocation error */
    }

    /* Split the PATH environment variable into directories */
    directories = tokenize_path(data);
    if (!directories || !directories[0]) {
        errno = ENOENT; /* Invalid or empty PATH environment variable */
        return errno;
    }

    /* Iterate through each directory in the PATH */
    for (index = 0; directories[index]; index++) {
        char *full_path;

        /* Construct the full path to the command in the current directory */
        full_path = str_concat(directories[index], data->tokens[0]);

        /* Check if the file exists, is not a directory, and has execute permissions */
        result = check_file(full_path);

        /* If the file was found and is executable, update the command name token */
        /* and free the allocated memory before returning */
        if (result == 0 || result == EACCES) {
            errno = 0;
            free(data->tokens[0]);
            data->tokens[0] = str_duplicate(full_path);
            free_array_of_pointers(directories);
            return result;
        }

        /* Free the constructed full path */
        free(full_path);
    }

    /* Free the modified command name token and the PATH directories array */
    free(data->tokens[0]);
    data->tokens[0] = NULL;
    free_array_of_pointers(directories);

    /* Return the error code from the last check */
    return ENOENT; /* Command not found */
}

/*
 * tokenize_path - Split the PATH environment variable into directories.
 * @data: Pointer to the program data structure.
 * Return: An array of path directories.
 */
char **tokenize_path(program_data *data) {
    int directory_count = 2; /* Initial count for current and parent directories */
    int index = 0;
    char *path;
    char **tokens = NULL;

    /* Get the PATH environment variable */
    path = env_get_key("PATH", data);
    if (!path || path[0] == '\0') {
        /* Invalid or empty PATH environment variable */
        return NULL;
    }

    /* Duplicate the PATH value */
    path = str_duplicate(path);

    /* Count the number of directories in the PATH */
 for (index = 0; path[index]; index++) {
        if (path[index] == ':') {
            directory_count++;
        }
    }

    // Allocate memory for the array of path directories
    tokens = malloc(sizeof(char *) * directory_count);

    // Tokenize the PATH and duplicate each directory token
    index = 0;
    tokens[index++] = str_duplicate(_strtok(path, ":"));
    while (tokens[index - 1]) {
        tokens[index++] = str_duplicate(_strtok(NULL, ":"));
    }

    // Free the duplicated PATH value
    free(path);
    path = NULL;

    return tokens;
}

/**
 * check_file - check if a file exists
 **/

#include "shell.h"

int verify_file(char *path);

/**
 * locate_program - searches for a program within the specified path
 * @prog_data: pointer to the program's data
 * Return: 0 if successful, an error code otherwise
 */
int locate_program(program_data *prog_data) {
	int idx = 0, res_code = 0;
	char **dirs;

	if (!prog_data->exec_name)
		return (2);

	/** Determine if it's a full path or an executable in the current directory */
	if (prog_data->exec_name[0] == '/' || prog_data->exec_name[0] == '.')
		return (verify_file(prog_data->exec_name));

	free(prog_data->args[0]);
	prog_data->args[0] = string_concat(string_duplicate("/"), prog_data->exec_name);
	if (!prog_data->args[0])
		return (2);

	dirs = fetch_path_tokens(prog_data); /* Explore in the PATH */

	if (!dirs || !dirs[0]) {
		errno = 127;
		return (127);
	}

	for (idx = 0; dirs[idx]; idx++) {
		dirs[idx] = string_concat(dirs[idx], prog_data->args[0]);
		res_code = verify_file(dirs[idx]);
		if (res_code == 0 || res_code == 126) {
			errno = 0;
			free(prog_data->args[0]);
			prog_data->args[0] = string_duplicate(dirs[idx]);
			free_str_array(dirs);
			return (res_code);
		}
	}

	free(prog_data->args[0]);
	prog_data->args[0] = NULL;
	free_str_array(dirs);
	return (res_code);
}

/**
 * fetch_path_tokens - separates the PATH variable into directories
 * @prog_data: pointer to the program's data
 * Return: an array of path directories
 */
char **fetch_path_tokens(program_data *prog_data) {
	int idx = 0;
	int dir_count = 2;
	char **tokens = NULL;
	char *path;

	/* Acquire the PATH value */
	path = get_env_var("PATH", prog_data);
	if ((path == NULL) || path[0] == '\0') {
		return (NULL);
	}

	path = string_duplicate(path);

	/* Count the number of directories in the PATH */
	for (idx = 0; path[idx]; idx++) {
		if (path[idx] == ':')
			dir_count++;
	}

	/* Reserve space for the array of pointers */
	tokens = malloc(sizeof(char *) * dir_count);

	/* Tokenize and replicate each path token */
	idx = 0;
	tokens[idx] = string_duplicate(_strtok(path, ":"));
	while (tokens[idx++]) {
		tokens[idx] = string_duplicate(_strtok(NULL, ":"));
	}

	free(path);
	path = NULL;
	return (tokens);
}

/**
 * verify_file - checks the existence, non-directory, and execution permissions of a file
 * @file_path: pointer to the full file name
 * Return: 0 on success, or an error code if it exists.
 */
int verify_file(char *file_path) {
	struct stat file_info;

	if (stat(file_path, &file_info) != -1) {
		if (S_ISDIR(file_info.st_mode) || access(file_path, X_OK)) {
			errno = 126;
			return (126);
		}
		return (0);
	}
	/* The file doesn't exist */
	errno = 127;
	return (127);
}

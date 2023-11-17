#include "shell.h"

<<<<<<< HEAD
/**
 * path_execute - executes a command in the path
 * @command: full path to the command
 * @vars: pointer to struct of variables
 *
 * Return: 0 on succcess, 1 on failure
 */
int path_execute(char *command, vars_t *vars)
{
	pid_t child_pid;

	if (access(command, X_OK) == 0)
	{
		child_pid = fork();
		if (child_pid == -1)
			print_error(vars, NULL);
		if (child_pid == 0)
		{
			if (execve(command, vars->av, vars->env) == -1)
				print_error(vars, NULL);
		}
		else
		{
			wait(&vars->status);
			if (WIFEXITED(vars->status))
				vars->status = WEXITSTATUS(vars->status);
			else if (WIFSIGNALED(vars->status) && WTERMSIG(vars->status) == SIGINT)
				vars->status = 130;
			return (0);
		}
		vars->status = 127;
		return (1);
	}
	else
	{
		print_error(vars, ": Permission denied\n");
		vars->status = 126;
	}
	return (0);
}

/**
 * find_path - finds the PATH variable
 * @env: array of environment variables
 *
 * Return: pointer to the node that contains the PATH, or NULL on failure
 */
char *find_path(char **env)
{
	char *path = "PATH=";
	unsigned int i, j;

	for (i = 0; env[i] != NULL; i++)
	{
		for (j = 0; j < 5; j++)
			if (path[j] != env[i][j])
				break;
		if (j == 5)
			break;
	}
	return (env[i]);
=======
int check_file(char *full_path);

/**
 * find_program - find a program in path
 * @data: a pointer to the program's data
 * Return: 0 if success, errcode otherwise
 */

int find_program(data_of_program *data)
{
	int i = 0, ret_code = 0;
	char **directories;

	if (!data->command_name)
		return (2);

	/**if is a full_path or an executable in the same path */
	if (data->command_name[0] == '/' || data->command_name[0] == '.')
		return (check_file(data->command_name));

	free(data->tokens[0]);
	data->tokens[0] = str_concat(str_duplicate("/"), data->command_name);
	if (!data->tokens[0])
		return (2);

	directories = tokenize_path(data);/* search in the PATH */

	if (!directories || !directories[0])
	{
		errno = 127;
		return (127);
	}
	for (i = 0; directories[i]; i++)
	{/* appends the function_name to path */
		directories[i] = str_concat(directories[i], data->tokens[0]);
		ret_code = check_file(directories[i]);
		if (ret_code == 0 || ret_code == 126)
		{/* the file was found, is not a directory and has execute permisions*/
			errno = 0;
			free(data->tokens[0]);
			data->tokens[0] = str_duplicate(directories[i]);
			free_array_of_pointers(directories);
			return (ret_code);
		}
	}
	free(data->tokens[0]);
	data->tokens[0] = NULL;
	free_array_of_pointers(directories);
	return (ret_code);
}

/**
 * tokenize_path - tokenize the path in directories
 * @data: a pointer to the program's data
 * Return: array of path directories
 */

char **tokenize_path(data_of_program *data)
{
	int i = 0;
	int counter_directories = 2;
	char **tokens = NULL;
	char *PATH;

	/* get the PATH value*/
	PATH = env_get_key("PATH", data);
	if ((PATH == NULL) || PATH[0] == '\0')
	{/*path not found*/
		return (NULL);
	}

	PATH = str_duplicate(PATH);

	/* find the number of directories in the PATH */
	for (i = 0; PATH[i]; i++)
	{
		if (PATH[i] == ':')
			counter_directories++;
	}

	/* reserve space for the array of pointers */
	tokens = malloc(sizeof(char *) * counter_directories);

	/*tokenize and duplicate each token of path*/
	i = 0;
	tokens[i] = str_duplicate(_strtok(PATH, ":"));
	while (tokens[i++])
	{
		tokens[i] = str_duplicate(_strtok(NULL, ":"));
	}

	free(PATH);
	PATH = NULL;
	return (tokens);
>>>>>>> 5c594c349578887eb5977e48f8738346d119b6d0

}

/**
<<<<<<< HEAD
 * check_for_path - checks if the command is in the PATH
 * @vars: variables
 *
 * Return: void
 */
void check_for_path(vars_t *vars)
{
	char *path, *path_dup = NULL, *check = NULL;
	unsigned int i = 0, r = 0;
	char **path_tokens;
	struct stat buf;

	if (check_for_dir(vars->av[0]))
		r = execute_cwd(vars);
	else
	{
		path = find_path(vars->env);
		if (path != NULL)
		{
			path_dup = _strdup(path + 5);
			path_tokens = tokenize(path_dup, ":");
			for (i = 0; path_tokens && path_tokens[i]; i++, free(check))
			{
				check = _strcat(path_tokens[i], vars->av[0]);
				if (stat(check, &buf) == 0)
				{
					r = path_execute(check, vars);
					free(check);
					break;
				}
			}
			free(path_dup);
			if (path_tokens == NULL)
			{
				vars->status = 127;
				new_exit(vars);
			}
		}
		if (path == NULL || path_tokens[i] == NULL)
		{
			print_error(vars, ": not found\n");
			vars->status = 127;
		}
		free(path_tokens);
	}
	if (r == 1)
		new_exit(vars);
}

/**
 * execute_cwd - executes the command in the current working directory
 * @vars: pointer to struct of variables
 *
 * Return: 0 on success, 1 on failure
 */
int execute_cwd(vars_t *vars)
{
	pid_t child_pid;
	struct stat buf;

	if (stat(vars->av[0], &buf) == 0)
	{
		if (access(vars->av[0], X_OK) == 0)
		{
			child_pid = fork();
			if (child_pid == -1)
				print_error(vars, NULL);
			if (child_pid == 0)
			{
				if (execve(vars->av[0], vars->av, vars->env) == -1)
					print_error(vars, NULL);
			}
			else
			{
				wait(&vars->status);
				if (WIFEXITED(vars->status))
					vars->status = WEXITSTATUS(vars->status);
				else if (WIFSIGNALED(vars->status) && WTERMSIG(vars->status) == SIGINT)
					vars->status = 130;
				return (0);
			}
			vars->status = 127;
			return (1);
		}
		else
		{
			print_error(vars, ": Permission denied\n");
			vars->status = 126;
		}
			return (0);
	}
	print_error(vars, ": not found\n");
	vars->status = 127;
	return (0);
}

/**
 * check_for_dir - checks if the command is a part of a path
 * @str: command
 *
 * Return: 1 on success, 0 on failure
 */
int check_for_dir(char *str)
{
	unsigned int i;

	for (i = 0; str[i]; i++)
	{
		if (str[i] == '/')
			return (1);
	}
	return (0);
=======
 * check_file - checks if exists a file, if it is not a dairectory and
 * if it has excecution permisions for permisions.
 * @full_path: pointer to the full file name
 * Return: 0 on success, or error code if it exists.
 */

int check_file(char *full_path)
{
	struct stat sb;

	if (stat(full_path, &sb) != -1)
	{
		if (S_ISDIR(sb.st_mode) ||  access(full_path, X_OK))
		{
			errno = 126;
			return (126);
		}
		return (0);
	}
	/*if not exist the file*/
	errno = 127;
	return (127);
>>>>>>> 5c594c349578887eb5977e48f8738346d119b6d0
}

#include "shell.h"

/* global variable for ^C handling */
unsigned int sig_flag;

/**
 * sig_handler - handles ^C signal interupt
 * @uuv: unused variable (required for signal function prototype)
 *
 * Return: void
 */
static void sig_handler(int uuv)
{
	(void) uuv;
	if (sig_flag == 0)
		_puts("\n$ ");
	else
		_puts("\n");
}

/**
 * main - main function for the shell
 * @argc: number of arguments passed to main
 * @argv: array of arguments passed to main
 * @environment: array of environment variables
 *
 * Return: 0 or exit status, or ?
 */
int main(int argc __attribute__((unused)), char **argv, char **environment)
{
	size_t len_buffer = 0;
	unsigned int is_pipe = 0, i;
	vars_t vars = {NULL, NULL, NULL, 0, NULL, 0, NULL};

	vars.argv = argv;
	vars.env = make_env(environment);
	signal(SIGINT, sig_handler);
	if (!isatty(STDIN_FILENO))
		is_pipe = 1;
	if (is_pipe == 0)
		_puts("$ ");
	sig_flag = 0;
	while (getline(&(vars.buffer), &len_buffer, stdin) != -1)
	{
		sig_flag = 1;
		vars.count++;
		vars.commands = tokenize(vars.buffer, ";");
		for (i = 0; vars.commands && vars.commands[i] != NULL; i++)
		{
			vars.av = tokenize(vars.commands[i], "\n \t\r");
			if (vars.av && vars.av[0])
				if (check_for_builtins(&vars) == NULL)
					check_for_path(&vars);
		free(vars.av);
		}
		free(vars.buffer);
		free(vars.commands);
		sig_flag = 0;
		if (is_pipe == 0)
			_puts("$ ");
		vars.buffer = NULL;
	}
	if (is_pipe == 0)
		_puts("\n");
	free_env(vars.env);
	free(vars.buffer);
	exit(vars.status);
}

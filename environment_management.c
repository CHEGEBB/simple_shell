#include "shell.h"

/**
 * print_environment - prints the current environment
 * @info_struct: Structure containing potential arguments.
 *                Used to maintain constant function prototype.
 * Return: Always 0
 */
int print_environment(info_t *info_struct)
{
	print_list_str(info_struct->env);
	return (0);
}

/**
 * get_environment_value - gets the value of an environment variable
 * @info_struct: Structure containing potential arguments.
 * @var_name: environment variable name
 *
 * Return: the value of the environment variable
 */
char *get_environment_value(info_t *info_struct, const char *var_name)
{
	list_t *current_node = info_struct->env;
	char *value;

	while (current_node)
	{
		value = starts_with(current_node->str, var_name);
		if (value && *value)
			return (value);
		current_node = current_node->next;
	}
	return (NULL);
}

/**
 * set_environment_variable - Initialize a new environment variable,
 *                           or modify an existing one
 * @info_struct: Structure containing potential arguments.
 * Return: Always 0
 */
int set_environment_variable(info_t *info_struct)
{
	if (info_struct->argc != 3)
	{
		_eputs("Incorrect number of arguments\n");
		return (1);
	}
	if (_setenv(info_struct, info_struct->argv[1], info_struct->argv[2]))
		return (0);
	return (1);
}

/**
 * unset_environment_variable - Remove an environment variable
 * @info_struct: Structure containing potential arguments.
 * Return: Always 0
 */
int unset_environment_variable(info_t *info_struct)
{
	int i;

	if (info_struct->argc == 1)
	{
		_eputs("Too few arguments.\n");
		return (1);
	}
	for (i = 1; i < info_struct->argc; i++)
		_unsetenv(info_struct, info_struct->argv[i]);

	return (0);
}

/**
 * populate_environment_list - populates environment linked list
 * @info_struct: Structure containing potential arguments.
 * Return: Always 0
 */
int populate_environment_list(info_t *info_struct)
{
	list_t *current_node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_node_end(&current_node, environ[i], 0);
	info_struct->env = current_node;
	return (0);
}

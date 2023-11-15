#include "shell.h"

/**
 * @brief Retrieve the string array copy of the environment.
 *
 * Returns the string array copy of the environment stored in the information struct.
 *
 * @param info_struct: The structure containing potential arguments.
 * @return The environment string array.
 */
char **get_environment(info_t *info_struct)
{
	if (!info_struct->environment || info_struct->environment_changed)
	{
		info_struct->environment = list_to_strings(info_struct->env);
		info_struct->environment_changed = 0;
	}

	return (info_struct->environment);
}

/**
 * @brief Remove an environment variable.
 *
 * Removes the specified environment variable from the environment list.
 *
 * @param info_struct: The structure containing potential arguments.
 * @param env_var: The string representing the environment variable.
 * @return 1 on successful deletion, 0 otherwise.
 */
int remove_environment_variable(info_t *info_struct, char *env_var)
{
	list_t *current_node = info_struct->env;
	size_t index = 0;
	char *position;

	if (!current_node || !env_var)
		return (0);

	while (current_node)
	{
		position = starts_with(current_node->str, env_var);
		if (position && *position == '=')
		{
			info_struct->environment_changed = delete_node_at_index(&(info_struct->env), index);
			index = 0;
			current_node = info_struct->env;
			continue;
		}
		current_node = current_node->next;
		index++;
	}
	return (info_struct->environment_changed);
}

/**
 * @brief Set or modify an environment variable.
 *
 * Initializes a new environment variable or modifies an existing one.
 *
 * @param info_struct: The structure containing potential arguments.
 * @param env_var: The string representing the environment variable.
 * @param value: The string representing the environment variable value.
 * @return 0 on success, 1 on error.
 */
int set_environment_variable(info_t *info_struct, char *env_var, char *value)
{
	char *buffer = NULL;
	list_t *current_node;
	char *position;

	if (!env_var || !value)
		return (1);

	buffer = malloc(_strlen(env_var) + _strlen(value) + 2);
	if (!buffer)
		return (1);

	_strcpy(buffer, env_var);
	_strcat(buffer, "=");
	_strcat(buffer, value);

	current_node = info_struct->env;
	while (current_node)
	{
		position = starts_with(current_node->str, env_var);
		if (position && *position == '=')
		{
			free(current_node->str);
			current_node->str = buffer;
			info_struct->environment_changed = 1;
			return (0);
		}
		current_node = current_node->next;
	}
	add_node_end(&(info_struct->env), buffer, 0);
	free(buffer);
	info_struct->environment_changed = 1;

	return (0);
}

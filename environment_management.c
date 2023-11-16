#include "shell.h"

/**
 * custom_env_get_key - Gets the value of an environment variable.
 * @requested_key: The environment variable of interest.
 * @data: Struct for the program's data.
 * Return: A pointer to the value of the variable or NULL if it doesn't exist.
 */
char *custom_env_get_key(char *requested_key, data_of_program *data)
{
	int index, key_length = 0;

	/* Validate the arguments */
	if (requested_key == NULL || data->env == NULL)
		return (NULL);

	/* Obtain the length of the variable requested */
	key_length = str_length(requested_key);

	for (index = 0; data->env[index]; index++)
	{
		/* Iterates through the environ and check for coincidence of the name */
		if (str_compare(requested_key, data->env[index], key_length) &&
		    data->env[index][key_length] == '=')
		{
			/* Returns the value of the key (NAME=) when found */
			return (data->env[index] + key_length + 1);
		}
	}
	/* Returns NULL if the key is not found */
	return (NULL);
}

/**
 * custom_env_set_key - Overwrites the value of the environment variable
 *                      or creates it if it does not exist.
 * @variable_key: Name of the variable to set.
 * @new_value: New value.
 * @data: Struct for the program's data.
 * Return: 1 if the parameters are NULL, 2 if there is an error, or 0 if success.
 */
int custom_env_set_key(char *variable_key, char *new_value, data_of_program *data)
{
	int index, key_length = 0, is_new_key = 1;

	/* Validate the arguments */
	if (variable_key == NULL || new_value == NULL || data->env == NULL)
		return (1);

	/* Obtain the length of the variable requested */
	key_length = str_length(variable_key);

	for (index = 0; data->env[index]; index++)
	{
		/* Iterates through the environ and checks for coincidence of the name */
		if (str_compare(variable_key, data->env[index], key_length) &&
		    data->env[index][key_length] == '=')
		{
			/* If the key already exists, free the entire variable */
			is_new_key = 0;
			free(data->env[index]);
			break;
		}
	}

	/* Create a string of the form key=value */
	data->env[index] = str_concat(str_duplicate(variable_key), "=");
	data->env[index] = str_concat(data->env[index], new_value);

	if (is_new_key)
	{
		/* If the variable is new, create it at the end of the actual list */
		/* and set the next position to NULL */
		data->env[index + 1] = NULL;
	}
	return (0);
}

/**
 * custom_env_remove_key - Remove a key from the environment.
 * @key_to_remove: The key to remove.
 * @data: The structure of the program's data.
 * Return: 1 if the key was removed, 0 if the key does not exist.
 */
int custom_env_remove_key(char *key_to_remove, data_of_program *data)
{
	int index, key_length = 0;

	/* Validate the arguments */
	if (key_to_remove == NULL || data->env == NULL)
		return (0);

	/* Obtain the length of the variable requested */
	key_length = str_length(key_to_remove);

	for (index = 0; data->env[index]; index++)
	{
		/* Iterates through the environ and checks for coincidences */
		if (str_compare(key_to_remove, data->env[index], key_length) &&
		    data->env[index][key_length] == '=')
		{
			/* If the key exists, remove it */
			free(data->env[index]);

			/* Move the other keys one position down */
			index++;
			for (; data->env[index]; index++)
			{
				data->env[index - 1] = data->env[index];
			}

			/* Set the NULL value at the new end of the list */
			data->env[index - 1] = NULL;
			return (1);
		}
	}
	return (0);
}

/**
 * print_environment - Prints the current environment.
 * @data: Struct for the program's data.
 * Return: Nothing.
 */
void print_environment(data_of_program *data)
{
	int k;

	for (k = 0; data->env[k]; k++)
	{
		_print(data->env[k]);
		_print("\n");
	}
}

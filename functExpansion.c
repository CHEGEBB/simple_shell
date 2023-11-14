#include "shell.h"

/**
 * perform_variable_expansion - Expand variables
 * @app_data: Pointer to the application's data structure
 *
 * Return: No return value, but sets errno.
 */
void perform_variable_expansion(application_data *app_data)
{
	int k, l;
	char input_line[BUFFER_SIZE] = {0}, expansion_buffer[BUFFER_SIZE] = {'\0'}, *temp;

	if (app_data->user_input == NULL)
		return;

	buffer_append(input_line, app_data->user_input);

	for (k = 0; input_line[k]; k++)
	{
		if (input_line[k] == '#')
		{
			// Handle '#' case
			input_line[k--] = '\0';
		}
		else if (input_line[k] == '$' && input_line[k + 1] == '?')
		{
			// Handle '$?' case
			input_line[k] = '\0';
			long_to_string(errno, expansion_buffer, 10);
			buffer_append(input_line, expansion_buffer);
			buffer_append(input_line, app_data->user_input + k + 2);
		}
		else if (input_line[k] == '$' && input_line[k + 1] == '$')
		{
			// Handle '$$' case
			input_line[k] = '\0';
			long_to_string(getpid(), expansion_buffer, 10);
			buffer_append(input_line, expansion_buffer);
			buffer_append(input_line, app_data->user_input + k + 2);
		}
		else if (input_line[k] == '$' && (input_line[k + 1] == ' ' || input_line[k + 1] == '\0'))
		{
			// Handle '$' followed by space or end of string
			continue;
		}
		else if (input_line[k] == '$')
		{
			// Handle general '$' case
			for (l = 1; input_line[k + l] && input_line[k + l] != ' '; l++)
				expansion_buffer[l - 1] = input_line[k + l];
			temp = get_environment_variable(expansion_buffer, app_data);
			input_line[k] = '\0', expansion_buffer[0] = '\0';
			buffer_append(expansion_buffer, input_line + k + l);
			temp ? buffer_append(input_line, temp) : 1;
			buffer_append(input_line, expansion_buffer);
		}
	}

	if (!string_compare(app_data->user_input, input_line, 0))
	{
		free(app_data->user_input);
		app_data->user_input = string_duplicate(input_line);
	}
}

/**
 * perform_alias_expansion - Expand aliases
 * @app_data: Pointer to the application's data structure
 *
 * Return: No return value, but sets errno.
 */
void perform_alias_expansion(application_data *app_data)
{
	int k, l, was_expanded = 0;
	char input_line[BUFFER_SIZE] = {0}, expansion_buffer[BUFFER_SIZE] = {'\0'}, *temp;

	if (app_data->user_input == NULL)
		return;

	buffer_append(input_line, app_data->user_input);

	for (k = 0; input_line[k]; k++)
	{
		for (l = 0; input_line[k + l] && input_line[k + l] != ' '; l++)
			expansion_buffer[l] = input_line[k + l];
		expansion_buffer[l] = '\0';

		temp = get_alias(app_data, expansion_buffer);
		if (temp)
		{
			expansion_buffer[0] = '\0';
			buffer_append(expansion_buffer, input_line + k + l);
			input_line[k] = '\0';
			buffer_append(input_line, temp);
			input_line[string_length(input_line)] = '\0';
			buffer_append(input_line, expansion_buffer);
			was_expanded = 1;
		}
		break;
	}

	if (was_expanded)
	{
		free(app_data->user_input);
		app_data->user_input = string_duplicate(input_line);
	}
}

/**
 * buffer_append - Append string at the end of the buffer
 * @buffer: Buffer to be filled
 * @str_to_add: String to be copied into the buffer
 * Return: The length of the resulting string in the buffer.
 */
int buffer_append(char *buffer, char *str_to_add)
{
	int buffer_length, l;

	buffer_length = string_length(buffer);
	for (l = 0; str_to_add[l]; l++)
	{
		buffer[buffer_length + l] = str_to_add[l];
	}
	buffer[buffer_length + l] = '\0';
	return (buffer_length + l);
}

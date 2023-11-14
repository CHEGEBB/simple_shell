#include "shell.h"

/**
 * release_recurring_data - Release resources used in each iteration
 * @app_data: The data structure for the application
 * Return: No return value
 */
void release_recurring_data(application_data *app_data)
{
	if (app_data->parsed_data)
		free_pointer_array(app_data->parsed_data);
	if (app_data->user_input)
		free(app_data->user_input);
	if (app_data->current_command)
		free(app_data->current_command);

	app_data->user_input = NULL;
	app_data->current_command = NULL;
	app_data->parsed_data = NULL;
}

/**
 * release_all_data - Release all resources allocated for the application
 * @app_data: The data structure for the application
 * Return: No return value
 */
void release_all_data(application_data *app_data)
{
	if (app_data->file_handle != 0)
	{
		if (close(app_data->file_handle))
			handle_error(app_data->program_name);
	}
	release_recurring_data(app_data);
	free_pointer_array(app_data->environment_vars);
	free_pointer_array(app_data->alias_entries);
}

/**
 * free_pointer_array - Free each pointer in an array of pointers and the array itself
 * @ptr_array: Array of pointers
 * Return: No return value
 */
void free_pointer_array(char **ptr_array)
{
	int index;

	if (ptr_array != NULL)
	{
		for (index = 0; ptr_array[index]; index++)
			free(ptr_array[index]);

		free(ptr_array);
		ptr_array = NULL;
	}
}

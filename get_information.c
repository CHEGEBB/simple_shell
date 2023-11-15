#include "shell.h"

/**
 * reset_data_struct - initializes data_t structure
 * @data: structure address
 */
void reset_data_struct(data_t *data)
{
	data->arg = NULL;
	data->args = NULL;
	data->path_loc = NULL;
	data->arg_count = 0;
}

/**
 * initialize_data_struct - initializes data_t structure
 * @data: structure address
 * @av: argument vector
 */
void initialize_data_struct(data_t *data, char **av)
{
	int index = 0;

	data->filename = av[0];
	if (data->arg)
	{
		data->args = split_string(data->arg, " \t");
		if (!data->args)
		{
			data->args = malloc(sizeof(char *) * 2);
			if (data->args)
			{
				data->args[0] = duplicate_string(data->arg);
				data->args[1] = NULL;
			}
		}
		for (index = 0; data->args && data->args[index]; index++)
			;
		data->arg_count = index;

		alter_aliases(data);
		alter_variables(data);
	}
}

/**
 * release_data_struct - frees data_t structure fields
 * @data: structure address
 * @free_all: true if freeing all fields
 */
void release_data_struct(data_t *data, int free_all)
{
	free_string_array(data->args);
	data->args = NULL;
	free(data->path_loc);
	data->path_loc = NULL;

	if (free_all)
	{
		if (!data->cmd_buf)
			free(data->arg);
		if (data->env_list)
			free_list(&(data->env_list));
		if (data->history_list)
			free_list(&(data->history_list));
		if (data->alias_list)
			free_list(&(data->alias_list));
		free_string_array(data->env_vars);
		data->env_vars = NULL;
		free_buffer((void **)data->cmd_buf);
		if (data->read_fd > 2)
			close(data->read_fd);
		_putchar(BUF_FLUSH);
	}
}

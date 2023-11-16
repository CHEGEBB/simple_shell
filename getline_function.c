#include "shell.h"

/**
 * _custom_getline - Read one line from the prompt with enhanced features.
 * @data: Struct for the program's data.
 *
 * Return: Number of bytes read.
 */
int _custom_getline(data_of_program *data)
{
	char buff[BUFFER_SIZE] = {'\0'};
	static char *command_list[10] = {NULL};
	static char operator_array[10] = {'\0'};
	ssize_t bytes_read, l = 0;

	/* Check if there are no more commands in the array */
	/* and check the logical operators */
	if (!command_list[0] || (operator_array[0] == '&' && errno != 0) ||
	    (operator_array[0] == '|' && errno == 0))
	{
		/* Free the memory allocated in the array if it exists */
		for (l = 0; command_list[l]; l++)
		{
			free(command_list[l]);
			command_list[l] = NULL;
		}

		/* Read from the file descriptor into buff */
		bytes_read = read(data->file_descriptor, &buff, BUFFER_SIZE - 1);
		if (bytes_read == 0)
			return (-1);

		/* Split lines for '\n' or ';' */
		l = 0;
		do {
			command_list[l] = str_duplicate(_strtok(l ? NULL : buff, "\n;"));
			/* Check and split for '&&' and '||' operators */
			l = check_and_split_logic(command_list, l, operator_array);
		} while (command_list[l++]);
	}

	/* Obtain the next command (command 0) and remove it from the array */
	data->input_line = command_list[0];
	for (l = 0; command_list[l]; l++)
	{
		command_list[l] = command_list[l + 1];
		operator_array[l] = operator_array[l + 1];
	}

	return (str_length(data->input_line));
}

/**
 * check_and_split_logic - Check and split for '&&' and '||' operators.
 * @command_list: Array of commands.
 * @l: Index in the command_list to be checked.
 * @operator_array: Array of logical operators for each previous command.
 *
 * Return: Index of the last command in the command_list.
 */
int check_and_split_logic(char *command_list[], int l, char operator_array[])
{
	char *temp = NULL;
	int k;

	/* Check for the '&' char in the command line */
	for (k = 0; command_list[l] != NULL && command_list[l][k]; k++)
	{
		if (command_list[l][k] == '&' && command_list[l][k + 1] == '&')
		{
			/* Split the line when '&&' is found */
			temp = command_list[l];
			command_list[l][k] = '\0';
			command_list[l] = str_duplicate(command_list[l]);
			command_list[l + 1] = str_duplicate(temp + k + 2);
			l++;
			operator_array[l] = '&';
			free(temp);
			k = 0;
		}
		if (command_list[l][k] == '|' && command_list[l][k + 1] == '|')
		{
			/* Split the line when '||' is found */
			temp = command_list[l];
			command_list[l][k] = '\0';
			command_list[l] = str_duplicate(command_list[l]);
			command_list[l + 1] = str_duplicate(temp + k + 2);
			l++;
			operator_array[l] = '|';
			free(temp);
			k = 0;
		}
	}
	return (l);
}

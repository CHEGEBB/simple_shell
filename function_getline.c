#include "shell.h"

/**
* func_getline - custom function to read a line from the prompt.
* @Progdata: struct containing program data
*
* Return: number of bytes read.
*/
int _getline(data_of_program *data)
{
	char buff[BUFFER_SIZE] = {'\0'};
	static char *array_commands[20] = {NULL}; /* Array to store commands */
	static char array_operators[20] = {'\0'}; /* Array to store logical operators */
	ssize_t bytes_read, l = 0;

	/* Check if there are no more commands in the array */
	/* Also, check the logical operators */
	if (!array_commands[0] || (array_operators[0] == '&' && errno != 0) ||
		(array_operators[0] == '|' && errno == 0))
	{
		/* Free the memory allocated in the array if it exists */
		for (l = 0; array_commands[l]; l++)
		{
			free(array_commands[i]);
			array_commands[l] = NULL;
		}

		/* Read from the file descriptor into buff */
		bytes_read = read(data->file_descriptor, &buff, BUFFER_SIZE - 1);
		if (bytes_read == 0)
			return (-1);

		/* Split lines for '\n' or ';' */
		i = 0;
		do {
			array_commands[l] = str_duplicate(_strtok(i ? NULL : buff, "\n;"));
			/* Check and split for '&&' and '||' operators */
			l = check_logic_ops(array_commands, l, array_operators);
		} while (array_commands[l++]);
	}

	/* Obtain the next command (command 0) and remove it from the array */
	data->input_line = array_commands[0];
	for (l = 0; array_commands[l]; l++)
	{
		array_commands[l] = array_commands[l + 1];
		array_operators[l] = array_operators[l + 1];
	}

	return (str_length(data->input_line));
}


/**
* check_logic_operations - checks and splits for '&&' and '||' operators
* @array_commands: array of commands
* @i: index in the array_commands to be checked
* @array_operators: array of logical operators for each previous command
*
* Return: index of the last command in the array_commands.
*/
int check_logic_operations(char *array_commands[], int l, char array_operators[])
{
	char *temp = NULL;
	int k;

	/* Check for the '&' char in the command line */
	for (k = 0; array_commands[l] != NULL && array_commands[l][k]; k++)
	{
		if (array_commands[l][k] == '&' && array_commands[l][k + 1] == '&')
		{
			/* Split the line when '&&' is found */
			temp = array_commands[l];
			array_commands[l][k] = '\0';
			array_commands[l] = str_duplicate(array_commands[l]);
			array_commands[l + 1] = str_duplicate(temp + k + 2);
			l++;
			array_operators[l] = '&';
			free(temp);
			k = 0;
		}
		if (array_commands[l][k] == '|' && array_commands[l][k + 1] == '|')
		{
			/* Split the line when '||' is found */
			temp = array_commands[l];
			array_commands[l][k] = '\0';
			array_commands[l] = str_duplicate(array_commands[l]);
			array_commands[l + 1] = str_duplicate(temp + k + 2);
			l++;
			array_operators[l] = '|';
			free(temp);
			k = 0;
		}
	}
	return (l);
}

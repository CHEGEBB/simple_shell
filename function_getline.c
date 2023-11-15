#include "shell.h"

/**
* _custom_getline - custom function to read a line from the prompt.
* @data: struct containing program data
*
* Return: number of bytes read.
*/
int _custom_getline(data_of_program *data)
{
	char buff[BUFFER_SIZE] = {'\0'};
	static char *array_commands[20] = {NULL}; /* Array to store commands */
	static char array_operators[20] = {'\0'}; /* Array to store logical operators */
	ssize_t bytes_read, i = 0;

	/* Check if there are no more commands in the array */
	/* Also, check the logical operators */
	if (!array_commands[0] || (array_operators[0] == '&' && errno != 0) ||
		(array_operators[0] == '|' && errno == 0))
	{
		/* Free the memory allocated in the array if it exists */
		for (i = 0; array_commands[i]; i++)
		{
			free(array_commands[i]);
			array_commands[i] = NULL;
		}

		/* Read from the file descriptor into buff */
		bytes_read = read(data->file_descriptor, &buff, BUFFER_SIZE - 1);
		if (bytes_read == 0)
			return (-1);

		/* Split lines for '\n' or ';' */
		i = 0;
		do {
			array_commands[i] = str_duplicate(_strtok(i ? NULL : buff, "\n;"));
			/* Check and split for '&&' and '||' operators */
			i = check_logic_ops(array_commands, i, array_operators);
		} while (array_commands[i++]);
	}

	/* Obtain the next command (command 0) and remove it from the array */
	data->input_line = array_commands[0];
	for (i = 0; array_commands[i]; i++)
	{
		array_commands[i] = array_commands[i + 1];
		array_operators[i] = array_operators[i + 1];
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
int check_logic_operations(char *array_commands[], int i, char array_operators[])
{
	char *temp = NULL;
	int j;

	/* Check for the '&' char in the command line */
	for (j = 0; array_commands[i] != NULL && array_commands[i][j]; j++)
	{
		if (array_commands[i][j] == '&' && array_commands[i][j + 1] == '&')
		{
			/* Split the line when '&&' is found */
			temp = array_commands[i];
			array_commands[i][j] = '\0';
			array_commands[i] = str_duplicate(array_commands[i]);
			array_commands[i + 1] = str_duplicate(temp + j + 2);
			i++;
			array_operators[i] = '&';
			free(temp);
			j = 0;
		}
		if (array_commands[i][j] == '|' && array_commands[i][j + 1] == '|')
		{
			/* Split the line when '||' is found */
			temp = array_commands[i];
			array_commands[i][j] = '\0';
			array_commands[i] = str_duplicate(array_commands[i]);
			array_commands[i + 1] = str_duplicate(temp + j + 2);
			i++;
			array_operators[i] = '|';
			free(temp);
			j = 0;
		}
	}
	return (i);
}

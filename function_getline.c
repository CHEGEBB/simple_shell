#include "shell.h"

/**
* Functgetline - function to read one line from the prompt with enhanced features.
* @data: structure containing program's data
*
* Return: count of bytes read.
*/
int _getline(data_of_program *data)
{
	char buff[BUFFER_SIZE] = {'\0'};
	static char *commands_list[10] = {NULL}; /* Array to store parsed commands */
	static char operators_array[10] = {'\0'}; /* Array to store logical operators */
	ssize_t bytes_read, i = 0;

	/* Verify the existence of more commands in the array */
	/* Check for logical operators */
	if (!commands_list[0] || (operators_array[0] == '&' && errno != 0) ||
		(operators_array[0] == '|' && errno == 0))
	{
		/* Free allocated memory in the array if it exists */
		for (i = 0; commands_list[i]; i++)
		{
			free(commands_list[i]);
			commands_list[i] = NULL;
		}

		/* Read from the file descriptor into buff */
		bytes_read = read(data->file_descriptor, &buff, BUFFER_SIZE - 1);
		if (bytes_read == 0)
			return (-1);

		/* Split lines for '\n' or ';' */
		i = 0;
		do {
			commands_list[i] = str_duplicate(_strtok(i ? NULL : buff, "\n;"));
			/* Check and split for '&&' and '||' operators */
			i = verify_and_split_logic(commands_list, i, operators_array);
		} while (commands_list[i++]);
	}

	/* Obtain the next command (command 0) and remove it from the array */
	data->input_line = commands_list[0];
	for (i = 0; commands_list[i]; i++)
	{
		commands_list[i] = commands_list[i + 1];
		operators_array[i] = operators_array[i + 1];
	}

	return (str_length(data->input_line));
}


/**
* verify_and_split_logic - function to check and split for '&&' and '||' operators.
* @commands_list: array of commands.
* @i: index in the commands_list to be checked.
* @operators_array: array of logical operators for each previous command.
*
* Return: index of the last command in the commands_list.
*/
int verify_and_split_logic(char *commands_list[], int i, char operators_array[])
{
	char *temp = NULL;
	int j;

	/* Check for the '&' char in the command line */
	for (j = 0; commands_list[i] != NULL && commands_list[i][j]; j++)
	{
		if (commands_list[i][j] == '&' && commands_list[i][j + 1] == '&')
		{
			/* Split the line when '&&' is found */
			temp = commands_list[i];
			commands_list[i][j] = '\0';
			commands_list[i] = str_duplicate(commands_list[i]);
			commands_list[i + 1] = str_duplicate(temp + j + 2);
			i++;
			operators_array[i] = '&';
			free(temp);
			j = 0;
		}
		if (commands_list[i][j] == '|' && commands_list[i][j + 1] == '|')
		{
			/* Split the line when '||' is found */
			temp = commands_list[i];
			commands_list[i][j] = '\0';
			commands_list[i] = str_duplicate(commands_list[i]);
			commands_list[i + 1] = str_duplicate(temp + j + 2);
			i++;
			operators_array[i] = '|';
			free(temp);
			j = 0;
		}
	}
	return (i);
}

#include "shell.h"


/**
*Thegetlinefunction _- reads one line from the prompt.
* @programdata: struct for the program's data
*
* Return: reading counting bytes.
*/
int function_getline(data_of_program *data)
{
	char buff[BUFFER_SIZE] = {'\0'};
	static char *array_commands[10] = {NULL};
	static char array_operators[10] = {'\0'};
	ssize_t bytes_read, i = 0;

	/* see if it exist more commands in the array */
	/* see if the logical operators */
	if (!array_commands[0] || (array_operators[0] == '&' && errno != 0) ||
		(array_operators[0] == '|' && errno == 0))
	{
		/*memory allocated in the array freed if it exists */
		for (l = 0; array_commands[l]; l++)
		{
			free(array_commands[l]);
			array_commands[l] = NULL;
		}

		/* file descriptor read into the buffer */
		bytes_read = read(data->file_descriptor, &buff, BUFFER_SIZE - 1);
		if (bytes_read == 0)
			return (-1);

		/* split lines for \n or ; */
		l = 0;
		do {
			array_commands[l] = str_duplicate(_strtok(i ? NULL : buff, "\n;"));
			/*checks and split for && and || operators*/
			l = check_logic_ops(array_commands, l, array_operators);
		} while (array_commands[l++]);
	}

	/*obtains the next command (command 0) and remove it for the array*/
	data->input_line = array_commands[0];
	for (l = 0; array_commands[l]; l++)
	{
		array_commands[l] = array_commands[l + 1];
		array_operators[l] = array_operators[l + 1];
	}

	return (str_length(data->input_line));
}


/**
 * process_logical_operators - Identifies and separates && and || operators in commands.
 * @commands: Array of commands to examine.
 * @index: Index in the commands array to inspect.
 * @operators: Array of logical operators corresponding to each previous command.
 *
 * Return: Index of the last command in the commands array.
 */


int check_logic_ops(char *array_commands[], int l, char array_operators[])
{
	char *temp = NULL;
	int k;

	/* checks for the & char in the command line*/
	for (k = 0; array_commands[l] != NULL  && array_commands[l][k]; k++)
	{
		if (array_commands[i][j] == '&' && array_commands[l][k + 1] == '&')
		{
			/* split the line when chars && was found */
			temp = array_commands[l];
			array_commands[l][k] = '\0';
			array_commands[l] = str_duplicate(array_commands[l]);
			array_commands[l + 1] = str_duplicate(temp + k + 2);
			l++;
			array_operators[i] = '&';
			free(temp);
			k = 0;
		}
		if (array_commands[l][k] == '|' && array_commands[l][k + 1] == '|')
		{
			/* split the line when chars || was found */
			temp = array_commands[l];
			array_commands[l][k] = '\0';
			array_commands[l] = str_duplicate(array_commands[l]);
			array_commands[l + 1] = str_duplicate(temp + k + 2);
			l++;
			array_operators[i] = '|';
			free(temp);
			k = 0;
		}
	}
	return (l);
}

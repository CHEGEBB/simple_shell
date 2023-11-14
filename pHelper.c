#include "shell.h"

/**
 * _print_stdout - writes an array of characters to standard output
 * @string: pointer to the array of characters
 * Return: the number of bytes written.
 * Upon error, -1 is returned, and errno is set appropriately.
 */
int _print_stdout(char *string)
{
	return (write(STDOUT_FILENO, string, str_length(string)));
}

/**
 * _print_stderr - writes an array of characters to standard error
 * @string: pointer to the array of characters
 * Return: the number of bytes written.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _print_stderr(char *string)
{
	return (write(STDERR_FILENO, string, str_length(string)));
}

/**
 * _print_error_message - writes an error message to standard error
 * @errorcode: error code to print
 * @data: a pointer to the program's data
 * Return: the number of bytes written.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _print_error_message(int errorcode, data_of_program *data)
{
	char counter_as_string[10] = {'\0'};

	long_to_string((long)data->exec_counter, counter_as_string, 10);

	if (errorcode == 2 || errorcode == 3)
	{
		_print_stderr(data->program_name);
		_print_stderr(": ");
		_print_stderr(counter_as_string);
		_print_stderr(": ");
		_print_stderr(data->tokens[0]);
		if (errorcode == 2)
			_print_stderr(": Illegal number: ");
		else
			_print_stderr(": can't cd to ");
		_print_stderr(data->tokens[1]);
		_print_stderr("\n");
	}
	else if (errorcode == 127)
	{
		_print_stderr(data->program_name);
		_print_stderr(": ");
		_print_stderr(counter_as_string);
		_print_stderr(": ");
		_print_stderr(data->command_name);
		_print_stderr(": not found\n");
	}
	else if (errorcode == 126)
	{
		_print_stderr(data->program_name);
		_print_stderr(": ");
		_print_stderr(counter_as_string);
		_print_stderr(": ");
		_print_stderr(data->command_name);
		_print_stderr(": Permission denied\n");
	}
	return (0);
}

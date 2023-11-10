#include "shell.h"

/**
 * separate_string - separates a string using a specified delimiter
 * @program_data: a pointer to the program's data
 * Return: an array containing the different parts of the string
 */
void separate_string(program_data *program_data)
{
	char *delim = " \t";
	int index_i, index_j, counter = 2, len;

	len = str_length(program_data->input_line);
	if (len) {
		if (program_data->input_line[len - 1] == '\n')
			program_data->input_line[len - 1] = '\0';
	}

	for (index_i = 0; program_data->input_line[index_i]; index_i++) {
		for (index_j = 0; delim[index_j]; index_j++) {
			if (program_data->input_line[index_i] == delim[index_j])
				counter++;
		}
	}

	program_data->tokens = malloc(counter * sizeof(char *));
	if (program_data->tokens == NULL) {
		perror(program_data->program_name);
		exit(errno);
	}
	index_i = 0;
	program_data->tokens[index_i] = str_duplicate(_strtok(program_data->input_line, delim));
	program_data->command_name = str_duplicate(program_data->tokens[0]);
	while (program_data->tokens[index_i++]) {
		program_data->tokens[index_i] = str_duplicate(_strtok(NULL, delim));
	}
}
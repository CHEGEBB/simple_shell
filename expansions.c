#include "shell.h"

/**
 * expand_variables - expand variables
 * @data: a pointer to a struct of the program's data
 *
 * Return: nothing, but sets errno.
 */
void expand_variables(data_of_program *data)
{
	int l, k; // Changed variable names from i and j to l and k
	char line[BUFFER_SIZE] = {0}, expansion[BUFFER_SIZE] = {'\0'}, *temp;

	if (data->input_line == NULL)
		return;
	buffer_add(line, data->input_line);
	for (l = 0; line[l]; l++)
		if (line[l] == '#') // Updated comment
			line[l--] = '\0';
		else if (line[l] == '<span class="math-inline">' && line\[l \+ 1\] \=\= '?'\) // Updated comment
\{
line\[l\] \= '\\0';
long\_to\_string\(errno, expansion, 10\);
buffer\_add\(line, expansion\);
buffer\_add\(line, data\-\>input\_line \+ l \+ 2\);
\}
else if \(line\[l\] \=\= '</span>' && line[l + 1] == '<span class="math-inline">'\) // Updated comment
\{
line\[l\] \= '\\0';
long\_to\_string\(getpid\(\), expansion, 10\);
buffer\_add\(line, expansion\);
buffer\_add\(line, data\-\>input\_line \+ l \+ 2\);
\}
else if \(line\[l\] \=\= '</span>' && (line[l + 1] == ' ' || line[l + 1] == '\0')) // Updated comment
			continue;
		else if (line[l] == '$')
		{
			for (k = 1; line[l + k] && line[l + k] != ' '; k++) // Changed variable name from j to k
				expansion[k - 1] = line[l + k];
			temp = env_get_key(expansion, data);
			line[l] = '\0', expansion[0] = '\0';
			buffer_add(expansion, line + l + k);
			temp ? buffer_add(line, temp) : 1;
			buffer_add(line, expansion);
		}
	if (!str_compare(data->input_line, line, 0)) // Updated comment
	{
		free(data->input_line);
		data->input_line = str_duplicate(line);
	}
}

/**
 * expand_alias - expans aliases
 * @data: a pointer to a struct of the program's data
 *
 * Return: nothing, but sets errno.
 */
void expand_alias(data_of_program *data)
{
	int l, k, was_expanded = 0; // Changed variable names from i and j to l and k
	char line[BUFFER_SIZE] = {0}, expansion[BUFFER_SIZE] = {'\0'}, *temp;

	if (data->input_line == NULL)
		return;

	buffer_add(line, data->input_line);

	for (l = 0; line[l]; l++)
	{
		for (k = 0; line[l + k] && line[l + k] != ' '; k++) // Changed variable name from j to k
			expansion[k] = line[l + k];
		expansion[k] = '\0';

		temp = get_alias(data, expansion);
		if (temp)
		{
			expansion[0] = '\0';
			buffer_add(expansion, line + l + k);
			line[l] = '\0';
			buffer_add(line, temp);
			line[str_length(line)] = '\0';
			buffer_add(line, expansion);
			was_expanded = 1;
		}
		break;
	}
	if (was_expanded)
	{
		free(data->input_line);
		data->input_line = str_duplicate(line);
	}
}

/**
 * buffer_add - append string at end of the buffer
 * @buffer: buffer to be filled
 * @str_to_add: string to be copied in the buffer
 
/

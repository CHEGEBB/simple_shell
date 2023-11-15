#include "shell.h"

/**
 * @brief Print a string to standard error.
 *
 * Prints the input string to the standard error output.
 *
 * @param error_str: The string to be printed.
 * @return Nothing.
 */
void print_error_string(char *error_str)
{
	int index = 0;

	if (!error_str)
		return;

	while (error_str[index] != '\0')
	{
		print_error_char(error_str[index]);
		index++;
	}
}

/**
 * @brief Write a character to standard error.
 *
 * Writes the given character to the standard error output.
 *
 * @param c: The character to print.
 * @return On success, 1.
 *         On error, -1 is returned, and errno is set appropriately.
 */
int write_error_char(char c)
{
	static int buffer_index;
	static char buffer[WRITE_BUFFER_SIZE];

	if (c == FLUSH_BUFFER || buffer_index >= WRITE_BUFFER_SIZE)
	{
		write(2, buffer, buffer_index);
		buffer_index = 0;
	}
	if (c != FLUSH_BUFFER)
		buffer[buffer_index++] = c;

	return (1);
}

/**
 * @brief Write a character to the given file descriptor.
 *
 * Writes the given character to the specified file descriptor.
 *
 * @param c: The character to print.
 * @param file_descriptor: The file descriptor to write to.
 * @return On success, 1.
 *         On error, -1 is returned, and errno is set appropriately.
 */
int write_to_file_descriptor(char c, int file_descriptor)
{
	static int buffer_index;
	static char buffer[WRITE_BUFFER_SIZE];

	if (c == FLUSH_BUFFER || buffer_index >= WRITE_BUFFER_SIZE)
	{
		write(file_descriptor, buffer, buffer_index);
		buffer_index = 0;
	}
	if (c != FLUSH_BUFFER)
		buffer[buffer_index++] = c;

	return (1);
}

/**
 * @brief Print a string to the specified file descriptor.
 *
 * Prints the input string to the specified file descriptor.
 *
 * @param output_str: The string to be printed.
 * @param file_descriptor: The file descriptor to write to.
 * @return The number of characters printed.
 */
int print_to_file_descriptor(char *output_str, int file_descriptor)
{
	int chars_printed = 0;

	if (!output_str)
		return (0);

	while (*output_str)
	{
		chars_printed += write_to_file_descriptor(*output_str++, file_descriptor);
	}

	return (chars_printed);
}

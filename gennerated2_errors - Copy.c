#include "shell.h"

/**
 * @brief Convert a string to an integer.
 *
 * Converts the input string to an integer, handling errors and checking for overflow.
 *
 * @param str: The string to be converted.
 * @return 0 if no numbers in the string, converted number otherwise, -1 on error.
 */
int convert_string_to_integer(char *str)
{
	int index;
	unsigned long int result = 0;

	if (*str == '+')
		str++;

	for (index = 0;  str[index] != '\0'; index++)
	{
		if (str[index] >= '0' && str[index] <= '9')
		{
			result *= 10;
			result += (str[index] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}

	return (result);
}

/**
 * @brief Print an error message.
 *
 * Prints an error message with information about the file, line, and command.
 *
 * @param info_struct: The information struct containing details about the command.
 * @param error_string: String containing specified error type.
 * @return Nothing.
 */
void print_error_message(info_t *info_struct, char *error_string)
{
	print_string_to_standard_error(info_struct->filename);
	print_string_to_standard_error(": ");
	print_integer_to_standard_error(info_struct->line_number, STDERR_FILENO);
	print_string_to_standard_error(": ");
	print_string_to_standard_error(info_struct->argv[0]);
	print_string_to_standard_error(": ");
	print_string_to_standard_error(error_string);
}

/**
 * @brief Print an integer to a file descriptor.
 *
 * Prints a decimal (integer) number (base 10) to the specified file descriptor.
 *
 * @param num: The integer to print.
 * @param fd: The file descriptor to write to.
 * @return The number of characters printed.
 */
int print_integer_to_file_descriptor(int num, int fd)
{
	int (*put_char_func)(char) = _putchar;
	int index, count = 0;
	unsigned int absolute_value, current_digit;

	if (fd == STDERR_FILENO)
		put_char_func = _eputchar;

	if (num < 0)
	{
		absolute_value = -num;
		put_char_func('-');
		count++;
	}
	else
		absolute_value = num;

	current_digit = absolute_value;

	for (index = 1000000000; index > 1; index /= 10)
	{
		if (absolute_value / index)
		{
			put_char_func('0' + current_digit / index);
			count++;
		}
		current_digit %= index;
	}

	put_char_func('0' + current_digit);
	count++;

	return (count);
}

/**
 * @brief Convert a number to a string.
 *
 * Converts the given number to a string in the specified base.
 *
 * @param num: The number to convert.
 * @param base: The base to convert the number to.
 * @param flags: Argument flags.
 * @return A string representation of the number.
 */
char *convert_number_to_string(long int num, int base, int flags)
{
	static char *char_array;
	static char buffer[50];
	char sign = 0;
	char *pointer;
	unsigned long number = num;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		number = -num;
		sign = '-';
	}

	char_array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	pointer = &buffer[49];
	*pointer = '\0';

	do
	{
		*--pointer = char_array[number % base];
		number /= base;
	} while (number != 0);

	if (sign)
		*--pointer = sign;

	return (pointer);
}

/**
 * @brief Remove comments from a string.
 *
 * Replaces the first instance of '#' with '\0' in the given string.
 *
 * @param input_string: Address of the string to modify.
 * @return Always 0.
 */
void remove_comments_from_string(char *input_string)
{
	int index;

	for (index = 0; input_string[index] != '\0'; index++)
		if (input_string[index] == '#' && (!index || input_string[index - 1] == ' '))
		{
			input_string[index] = '\0';
			break;
		}
}

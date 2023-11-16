#include "shell.h"

/**
 * @brief Determine if the shell is in interactive mode.
 *
 * Checks if the shell is running in interactive mode based on the input struct.
 *
 * @param information_struct: The structure containing relevant information.
 * @return 1 if in interactive mode, 0 otherwise.
 */
int check_interactive(info_t *information_struct)
{
	return (isatty(STDIN_FILENO) && information_struct->read_fd <= 2);
}

/**
 * @brief Check if a character is a delimiter.
 *
 * Determines if the given character is a delimiter by comparing it to a delimiter string.
 *
 * @param character: The character to check.
 * @param delimiter_str: The delimiter string.
 * @return 1 if the character is a delimiter, 0 otherwise.
 */
int is_character_delimiter(char character, char *delimiter_str)
{
	while (*delimiter_str)
		if (*delimiter_str++ == character)
			return (1);
	return (0);
}

/**
 * @brief Check if a character is alphabetic.
 *
 * Determines if the given character is alphabetic.
 *
 * @param character: The character to check.
 * @return 1 if the character is alphabetic, 0 otherwise.
 */
int check_alphabetic(int character)
{
	if ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 * @brief Convert a string to an integer.
 *
 * Converts the input string to an integer, considering the sign and numeric characters.
 *
 * @param string: The string to be converted.
 * @return 0 if no numbers in the string, the converted number otherwise.
 */
int convert_string_to_integer(char *string)
{
	int index, sign = 1, flag = 0, result;
	unsigned int numeric_result = 0;

	for (index = 0;  string[index] != '\0' && flag != 2; index++)
	{
		if (string[index] == '-')
			sign *= -1;

		if (string[index] >= '0' && string[index] <= '9')
		{
			flag = 1;
			numeric_result *= 10;
			numeric_result += (string[index] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	if (sign == -1)
		result = -numeric_result;
	else
		result = numeric_result;

	return (result);
}

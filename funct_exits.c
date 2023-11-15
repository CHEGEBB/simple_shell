#include "shell.h"

/**
 * @brief Copy a string up to a specified number of characters.
 *
 * Copies the source string to the destination string up to the given number of characters.
 *
 * @param destination_str: The destination string to be copied to.
 * @param source_str: The source string.
 * @param max_chars: The maximum number of characters to be copied.
 * @return The concatenated string.
 */
char *copy_string(char *destination_str, char *source_str, int max_chars)
{
	int index_dest = 0, index_src;
	char *start = destination_str;

	index_src = 0;
	while (source_str[index_src] != '\0' && index_src < max_chars - 1)
	{
		destination_str[index_dest] = source_str[index_src];
		index_dest++;
		index_src++;
	}
	if (index_src < max_chars)
	{
		int index_fill = index_src;
		while (index_fill < max_chars)
		{
			destination_str[index_fill] = '\0';
			index_fill++;
		}
	}
	return (start);
}

/**
 * @brief Concatenate two strings up to a specified number of bytes.
 *
 * Concatenates the second string to the first string up to the given number of bytes.
 *
 * @param first_str: The first string.
 * @param second_str: The second string.
 * @param max_bytes: The maximum number of bytes to be used.
 * @return The concatenated string.
 */
char *concatenate_strings(char *first_str, char *second_str, int max_bytes)
{
	int index_first = 0, index_second = 0;
	char *start = first_str;

	while (first_str[index_first] != '\0')
		index_first++;

	while (second_str[index_second] != '\0' && index_second < max_bytes)
	{
		first_str[index_first] = second_str[index_second];
		index_first++;
		index_second++;
	}
	if (index_second < max_bytes)
		first_str[index_first] = '\0';

	return (start);
}

/**
 * @brief Locate a character in a string.
 *
 * Locates the specified character in the given string.
 *
 * @param input_str: The string to be parsed.
 * @param target_char: The character to look for.
 * @return A pointer to the memory area containing the character.
 */
char *find_character(char *input_str, char target_char)
{
	do {
		if (*input_str == target_char)
			return (input_str);
	} while (*input_str++ != '\0');

	return (NULL);
}

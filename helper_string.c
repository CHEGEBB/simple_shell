#include "shell.h"

/**
 * stringLength - Returns the length of a string.
 * @str: Pointer to string.
 * Return: Length of the string.
 */
int stringLength(char *str)
{
	int len = 0;

	if (str == NULL)
		return (0);

	while (str[len++] != '\0')
	{
	}
	return (--len);
}

/**
 * stringDuplicate - Duplicates a string.
 * @str: String to be copied.
 * Return: Pointer to the array.
 */
char *stringDuplicate(char *str)
{
	char *result;
	int length, i;

	if (str == NULL)
		return (NULL);

	length = stringLength(str) + 1;

	result = malloc(sizeof(char) * length);

	if (result == NULL)
	{
		errno = ENOMEM;
		perror("Error");
		return (NULL);
	}
	for (i = 0; i < length ; i++)
	{
		result[i] = str[i];
	}

	return (result);
}

/**
 * stringCompare - Compare two strings.
 * @s1: String one, or the shorter.
 * @s2: String two, or the longer.
 * @n: Number of characters to be compared, 0 if infinite.
 * Return: 1 if the strings are equals, 0 if the strings are different.
 */
int stringCompare(char *s1, char *s2, int n)
{
	int iterator;

	if (s1 == NULL && s2 == NULL)
		return (1);

	if (s1 == NULL || s2 == NULL)
		return (0);

	if (n == 0) /* Infinite length */
	{
		if (stringLength(s1) != stringLength(s2))
			return (0);
		for (iterator = 0; s1[iterator]; iterator++)
		{
			if (s1[iterator] != s2[iterator])
				return (0);
		}
		return (1);
	}
	else /* If there is a number of chars to be compared */
	{
		for (iterator = 0; iterator < n ; iterator++)
		{
			if (s1[iterator] != s2[iterator])
			return (0);
		}
		return (1);
	}
}

/**
 * stringConcat - Concatenates two strings.
 * @s1: String to be concatenated.
 * @s2: String to be concatenated.
 * Return: Pointer to the array.
 */
char *stringConcat(char *s1, char *s2)
{
	char *result;
	int len1 = 0, len2 = 0;

	if (s1 == NULL)
		s1 = "";
	len1 = stringLength(s1);

	if (s2 == NULL)
		s2 = "";
	len2 = stringLength(s2);

	result = malloc(sizeof(char) * (len1 + len2 + 1));
	if (result == NULL)
	{
		errno = ENOMEM;
		perror("Error");
		return (NULL);
	}

	/* Copy of s1 */
	for (len1 = 0; s1[len1] != '\0'; len1++)
		result[len1] = s1[len1];
	free(s1);

	/* Copy of s2 */
	for (len2 = 0; s2[len2] != '\0'; len2++)
	{
		result[len1] = s2[len2];
		len1++;
	}

	result[len1] = '\0';
	return (result);
}

/**
 * stringReverse - Reverses a string.
 * @str: Pointer to string.
 * Return: void.
 */
void stringReverse(char *str)
{
	int i = 0, length = stringLength(str) - 1;
	char temp;

	while (i < length)
	{
		temp = str[i];
		str[i++] = str[length];
		str[length--] = temp;
	}
}

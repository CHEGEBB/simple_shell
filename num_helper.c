#include "shell.h"

/**
 * numToStr - converts a number to a string.
 * @num: number to be converted to a string.
 * @str: buffer to store the number as a string.
 * @base: base to convert the number
 *
 * Return: Nothing.
 */
void numToStr(long num, char *str, int base)
{
	int idx = 0, isNegative = 0;
	long quotient = num;
	char digits[] = {"0123456789abcdef"};

	if (quotient == 0)
		str[idx++] = '0';

	if (str[0] == '-')
		isNegative = 1;

	while (quotient)
	{
		if (quotient < 0)
			str[idx++] = digits[-(quotient % base)];
		else
			str[idx++] = digits[quotient % base];
		quotient /= base;
	}
	if (isNegative)
		str[idx++] = '-';

	str[idx] = '\0';
	strReverse(str);
}

/**
 * strToInt - convert a string to an integer.
 *
 * @s: pointer to the source string.
 * Return: int value of the string or 0.
 */
int strToInt(char *s)
{
	int sign = 1;
	unsigned int num = 0;

	/* 1 - analyze sign */
	while (!('0' <= *s && *s <= '9') && *s != '\0')
	{
		if (*s == '-')
			sign *= -1;
		if (*s == '+')
			sign *= +1;
		s++;
	}

	/* 2 - extract the number */
	while ('0' <= *s && *s <= '9' && *s != '\0')
	{
		num = (num * 10) + (*s - '0');
		s++;
	}
	return (num * sign);
}

/**
 * countChars - count the occurrences of a character in a string.
 *
 * @str: pointer to the source string.
 * @ch: character to be counted.
 * Return: number of occurrences.
 */
int countChars(char *str, char *ch)
{
	int i = 0, counter = 0;

	for (; str[i]; i++)
	{
		if (str[i] == ch[0])
			counter++;
	}
	return (counter);
}

#include "shell.h"

/**
 * long_to_string - Convert a long integer to a string representation in the specified base.
 * @number: The long integer to be converted.
 * @string: The buffer to store the converted string.
 * @base: The base for the conversion.
 * Return: void
 */
void long_to_string(long number, char *string, int base) {
    int index = 0;
    bool isNegative = number < 0;
    long quotient = number;
    char digits[] = "0123456789abcdef";

    if (number == 0) {
        string[index++] = '0';
    }

    if (isNegative) {
        string[index++] = '-';
    }

    while (quotient) {
        int digit;
        if (quotient < 0) {
            digit = -(quotient % base);
        } else {
            digit = quotient % base;
        }
        string[index++] = digits[digit];
        quotient /= base;
    }

    string[index] = '\0';
    str_reverse(string);
}

/**
 * _atoi - Convert an ASCII string to an integer value.
 * @str: The pointer to the string to be converted.
 * Return: The converted integer value.
 */
int _atoi(char *str) {
    int sign = 1;
    unsigned int result = 0;

    // Skip whitespace and leading signs
    while (*str && !isdigit(*str) && *str != '+' && *str != '-') {
        str++;
    }

    // Check for leading sign
    if (*str == '+') {
        sign = 1;
        str++;
    } else if (*str == '-') {
        sign = -1;
        str++;
    }

    // Convert digits
    while (isdigit(*str)) {
        result = result * 10 + (*str - '0');
        str++;
    }

    return result * sign;
}

/**
 * count_characters - Count the occurrences of a specific character in a string.
 * @string: The pointer to the string to be searched.
 * @character: The character to count occurrences of.
 * Return: The number of occurrences of the character in the string.
 */
int count_characters(char *string, char character) {
    int count = 0;

    while (*string) {
        if (*string == character) {
            count++;
        }
        string++;
    }

    return count;
}

#include "shell.h"

/**
 * str_length - determines the length of a string.
 * @str: pointer to the string.
 * Return: length of the string.
 */
int str_length(char *str) {
    if (!str) return 0;

    int length = 0;
    while (str[length++] != '\0');
    return length - 1; // Adjust for null terminator
}

/**
 * str_duplicate - creates a copy of a string.
 * @str: string to be duplicated.
 * Return: pointer to the copied string.
 */
char *str_duplicate(char *str) {
    if (!str) return NULL;

    int length = str_length(str) + 1; // Account for null terminator

    char *dup = malloc(sizeof(char) * length);
    if (!dup) {
        errno = ENOMEM;
        perror("Error");
        return NULL;
    }

    for (int i = 0; i < length; i++) {
        dup[i] = str[i];
    }

    return dup;
}

/**
 * str_compare - compares two strings.
 * @str1: first string.
 * @str2: second string.
 * @num: number of characters to compare (0 for entire strings).
 * Return: 1 if strings are equal, 0 if they differ.
 */
int str_compare(char *str1, char *str2, int num) {
    if (!str1 && !str2) return 1; // Both strings are empty

    if (!str1 || !str2) return 0; // One string is empty

    if (num == 0) { // Comparing entire strings
        if (str_length(str1) != str_length(str2)) return 0;

        for (int i = 0; str1[i]; i++) {
            if (str1[i] != str2[i]) return 0;
        }

        return 1;
    } else { // Comparing specified number of characters
        for (int i = 0; i < num; i++) {
            if (str1[i] != str2[i]) return 0;
        }

        return 1;
    }
}

/**
 * str_concat - concatenates two strings.
 * @str1: first string.
 * @str2: second string.
 * Return: pointer to the concatenated string.
 */
char *str_concat(char *str1, char *str2) {
    int len1 = 0, len2 = 0;

    if (!str1) str1 = ""; // Handle empty strings
    len1 = str_length(str1);

    if (!str2) str2 = ""; // Handle empty strings
    len2 = str_length(str2);

    char *concat = malloc(sizeof(char) * (len1 + len2 + 1));
    if (!concat) {
        errno = ENOMEM;
        perror("Error");
        return NULL;
    }

    // Copy first string
    for (len1 = 0; str1[len1] != '\0'; len1++) {
        concat[len1] = str1[len1];
    }
    free(str1);

    // Copy second string
    for (len2 = 0; str2[len2] != '\0'; len2++) {
        concat[len1] = str2[len2];
        len1++;
    }

    concat[len1] = '\0';
    return concat;
}

/**
 * str_reverse - reverses a string.
 * @str: pointer to the string.
 * Return: void.
 */
void str_reverse(char *str) {
    int i = 0, length = str_length(str) - 1;
    char temp;

    while (i < length) {
        temp = str[i];
        str[i++] = str[length];
        str[length--] = temp;
    }
}

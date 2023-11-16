#include "shell.h"

/**
 * _strtok - separates strings with delimiters
 * @line: Pointer to the array received from getline.
 * @delim: Characters used to separate the string into parts.
 * Return: A pointer to the created token.
 */
char *_strtok(char *line, char *delim) {
    int k;
    static char *str;
    char *copystr;

    if (line != NULL) {
        str = line;
    }

    for (; *str != '\0'; str++) {
        for (k = 0; delim[k] != '\0'; k++) {
            if (*str == delim[k]) {
                break;
            }
        }

        if (delim[k] == '\0') {
            break;
        }
    }

    copystr = str;

    if (*copystr == '\0') {
        return (NULL);
    }

    for (; *str != '\0'; str++) {
        for (k = 0; delim[k] != '\0'; k++) {
            if (*str == delim[k]) {
                *str = '\0';
                str++;
                return (copystr);
            }
        }
    }

    return (copystr);
}

#include "shell.h"

/**
 * custom_strtok - Splits strings using specified delimiters
 *
 * @text: Pointer to the array received in getline
 * @delimiters: Characters marking off string segments
 *
 * Return: A pointer to the created token
 */
char *custom_strtok(char *text, char *delimiters) {
    int index;
    static char *text_ptr;
    char *copy_text;

    if (text != NULL) text_ptr = text;
    for (; *text_ptr != '\0'; text_ptr++) {
        for (index = 0; delimiters[index] != '\0'; index++)
            if (*text_ptr == delimiters[index]) break;
        if (delimiters[index] == '\0') break;
    }

    copy_text = text_ptr;
    if (*copy_text == '\0') return (NULL);

    for (; *text_ptr != '\0'; text_ptr++) {
        for (index = 0; delimiters[index] != '\0'; index++)
            if (*text_ptr == delimiters[index]) {
                *text_ptr = '\0';
                text_ptr++;
                return (copy_text);
            }
    }
    return (copy_text);
}

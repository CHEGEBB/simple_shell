#include "shell.h"

/**
 * free_recurrent_data - Free the dynamically allocated memory used for recurrent data structures.
 * @data: Pointer to the program data structure.
 * Return: void
 */
void free_recurrent_data(program_data *data) {
    if (data->tokens) {
        free_array_of_pointers(data->tokens);
    }

    if (data->input_line) {
        free(data->input_line);
    }

    if (data->command_name) {
        free(data->command_name);
    }

    data->input_line = NULL;
    data->command_name = NULL;
    data->tokens = NULL;
}

/**
 * free_all_data - Free all dynamically allocated memory used by the program data structure.
 * @data: Pointer to the program data structure.
 * Return: void
 */
void free_all_data(program_data *data) {
    if (data->file_descriptor != 0) {
        int close_result = close(data->file_descriptor);
        if (close_result != 0) {
            perror(data->program_name);
        }
    }

    free_recurrent_data(data);
    free_array_of_pointers(data->env);
    free_array_of_pointers(data->alias_list);
}

/**
 * free_array_of_pointers - Free all pointers in an array of pointers and the array itself.
 * @array: The array of pointers to be freed.
 * Return: void
 */
void free_array_of_pointers(char **array) {
    if (array != NULL) {
        int index;

        for (index = 0; array[index]; index++) {
            free(array[index]);
        }

        free(array);
        array = NULL;
    }
}

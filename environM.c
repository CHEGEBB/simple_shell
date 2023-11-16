#include "shell.h"

/**
 * Retrieves the value of an environment variable.
 *
 * @param variable_name The environment variable to retrieve the value for.
 * @param data The program's data structure.
 * @return A pointer to the variable's value, or NULL if it does not exist.
 */
char *get_environment_variable_value(char *variable_name, data_of_program *data) {
    if (!variable_name || !data->environment) {
        return NULL;
    }

    int variable_length = str_length(variable_name);

    for (int index = 0; data->environment[index]; ++index) {
        if (str_compare(variable_name, data->environment[index], variable_length) &&
            data->environment[index][variable_length] == '=') {
            return data->environment[index] + variable_length + 1;
        }
    }

    return NULL;
}

/**
 * Sets or creates an environment variable.
 *
 * @param variable_name The name of the environment variable to set.
 * @param new_value The new value to set.
 * @param data The program's data structure.
 * @return 1 if any of the parameters are NULL, 2 if there is an error, or 0 if successful.
 */
int set_environment_variable_value(char *variable_name, char *new_value, data_of_program *data) {
    if (!variable_name || !new_value || !data->environment) {
        return 1;
    }

    int variable_length = str_length(variable_name);

    bool is_new_variable = true;

    for (int index = 0; data->environment[index]; ++index) {
        if (str_compare(variable_name, data->environment[index], variable_length) &&
            data->environment[index][variable_length] == '=') {
            is_new_variable = false;
            free(data->environment[index]);
            break;
        }
    }

    data->environment[index] = str_concat(str_duplicate(variable_name), "=");
    data->environment[index] = str_concat(data->environment[index], new_value);

    if (is_new_variable) {
        data->environment[index + 1] = NULL;
    }

    return 0;
}

/**
 * Removes an environment variable.
 *
 * @param variable_name The key to remove.
 * @param data The program's data structure.
 * @return 1 if the key was removed, 0 if it does not exist.
 */
int remove_environment_variable(char *variable_name, data_of_program *data) {
    if (!variable_name || !data->environment) {
        return 0;
    }

    int variable_length = str_length(variable_name);

    for (int index = 0; data->environment[index]; ++index) {
        if (str_compare(variable_name, data->environment[index], variable_length) &&
            data->environment[index][variable_length] == '=') {
            free(data->environment[index]);

            for (int j = index + 1; data->environment[j]; ++j, ++index) {
                data->environment[index] = data->environment[j];
            }

            data->environment[index] = NULL;

            return 1;
        }
    }

    return 0;
}

/**
 * Prints the current environment.
 *
 * @param data The program's data structure.
 */
void print_environment(data_of_program *data) {
    for (int j = 0; data->environment[j]; ++j) {
        _print(data->environment[j]);
        _print("\n");
    }
}

#include "shell.h"

/**
 * function_getline - Reads one line from the prompt.
 * @data: Struct for the program's data.
 *
 * Return: Number of bytes read.
 */
int function_getline(data_of_program *data)
{
    char buff[BUFFER_SIZE] = {'\0'};
    static char *array_commands[10] = {NULL};
    static char array_operators[10] = {'\0'};
    ssize_t bytes_read, l = 0;

    /* Check if there are more commands in the array or logical operators */
    if (!array_commands[0] || (array_operators[0] == '&' && errno != 0) ||
        (array_operators[0] == '|' && errno == 0))
    {
        /* Memory allocated in the array freed if it exists */
        for (l = 0; array_commands[l]; l++)
        {
            free(array_commands[l]);
            array_commands[l] = NULL;
        }

        /* File descriptor read into the buffer */
        bytes_read = read(data->file_descriptor, &buff, BUFFER_SIZE - 1);
        if (bytes_read == 0)
            return (-1);

        /* Split lines for '\n' or ';' */
        l = 0;
        do {
            array_commands[l] = str_duplicate(_strtok(l ? NULL : buff, "\n;"));
            /* Checks and splits for && and || operators */
            l = check_logic_ops(array_commands, l, array_operators);
        } while (array_commands[l++]);
    }

    /* Obtain the next command (command 0) and remove it from the array */
    data->input_line = array_commands[0];
    for (l = 0; array_commands[l]; l++)
    {
        array_commands[l] = array_commands[l + 1];
        array_operators[l] = array_operators[l + 1];
    }

    return (str_length(data->input_line));
}

/**
 * check_logic_ops - Checks and splits for && and || operators.
 * @array_commands: Array of commands.
 * @l: Index in the array_commands to be checked.
 * @array_operators: Array of logical operators for each previous command.
 *
 * Return: Index of the last command in the array_commands.
 */
int check_logic_ops(char *array_commands[], int l, char array_operators[])
{
    char *temp = NULL;
    int k;

    /* Checks for the & char in the command line */
    for (k = 0; array_commands[l] != NULL && array_commands[l][k]; k++)
    {
        if (array_commands[l][k] == '&' && array_commands[l][k + 1] == '&')
        {
            /* Split the line when chars && are found */
            temp = array_commands[l];
            array_commands[l][k] = '\0';
            array_commands[l] = str_duplicate(array_commands[l]);
            array_commands[l + 1] = str_duplicate(temp + k + 2);
            l++;
            array_operators[l] = '&';
            free(temp);
            k = 0;
        }
        if (array_commands[l][k] == '|' && array_commands[l][k + 1] == '|')
        {
            /* Split the line when chars || are found */
            temp = array_commands[l];
            array_commands[l][k] = '\0';
            array_commands[l] = str_duplicate(array_commands[l]);
            array_commands[l + 1] = str_duplicate(temp + k + 2);
            l++;
            array_operators[l] = '|';
            free(temp);
            k = 0;
        }
    }
    return l;
}

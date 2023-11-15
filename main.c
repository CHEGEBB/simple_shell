#include "shell.h"

/**
 * main - Entry point for the shell program
 * @argc: Number of command-line arguments
 * @argv: Array of command-line argument strings
 * @env: Array of environment variable strings
 * Return: 0 on success
 */
int main(int argc, char *argv[], char *env[])
{
    data_of_program data_struct = {NULL}, *data = &data_struct;
    char *prompt = "";

    initialize_data(data, argc, argv, env);

    signal(SIGINT, handle_ctrl_c);

    if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO) && argc == 1)
    {
        /* We are in interactive mode in the terminal */
        errno = 2; // TODO: Clarify the purpose of setting errno to 2 here
        prompt = PROMPT_MSG;
    }
    errno = 0;
    sisifo(prompt, data);
    return 0;
}

/**
 * handle_ctrl_c - Signal handler for SIGINT (Ctrl+C)
 * @opr: Unused parameter (required by the prototype)
 */
void handle_ctrl_c(int opr UNUSED)
{
    _print("\n");
    _print(PROMPT_MSG);
}

/**
 * initialize_data - Initialize the data structure with program information
 * @data: Pointer to the structure of data
 * @argv: Array of arguments passed to the program execution
 * @env: Environment passed to the program execution
 * @argc: Number of values received from the command line
 */
void initialize_data(data_of_program *data, int argc, char *argv[], char **env)
{
    int i = 0;

    data->program_name = argv[0];
    data->input_line = NULL;
    data->command_name = NULL;
    data->exec_counter = 0;

    if (argc == 1)
        data->file_descriptor = STDIN_FILENO;
    else
    {
        data->file_descriptor = open(argv[1], O_RDONLY);
        if (data->file_descriptor == -1)
        {
            _printe(data->program_name);
            _printe(": 0: Can't open ");
            _printe(argv[1]);
            _printe("\n");
            exit(127);
        }
    }
    data->tokens = NULL;
    data->env = malloc(sizeof(char *) * 50);
    if (env)
    {
        for (; env[i]; i++)
        {
            data->env[i] = str_duplicate(env[i]);
        }
    }
    data->env[i] = NULL;
    env = data->env;

    data->alias_list = malloc(sizeof(char *) * 20);
    for (i = 0; i < 20; i++)
    {
        data->alias_list[i] = NULL;
    }
}

/**
 * sisifo - Infinite loop that displays the prompt and processes user input
 * @prompt: Prompt to be printed
 * @data: Pointer to the structure of data
 */
void sisifo(char *prompt, data_of_program *data)
{
    int error_code = 0, string_len = 0;

    while (++(data->exec_counter))
    {
        _print(prompt);
        error_code = string_len = _getline(data);

        if (error_code == EOF)
        {
            free_all_data(data);
            exit(errno);
        }
        if (string_len >= 1)
        {
            expand_alias(data);
            expand_variables(data);
            tokenize(data);
            if (data->tokens[0])
            {
                error_code = execute(data);
                if (error_code != 0)
                    _print_error(error_code, data);
            }
            free_recurrent_data(data);
        }
    }
}

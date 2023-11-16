#include "shell.h"

/**
 * main - Entry point to initialize program variables.
 * @argc: Number of arguments from the command line.
 * @argv: Array of arguments from the command line.
 * @env: Environment variables.
 * Return: Zero on success.
 */
int main(int argc, char *argv[], char *env[])
{
	data_of_program data_struct = {NULL}, *data = &data_struct;
	char *custom_prompt = "";

	initialize_program_data(data, argc, argv, env);

	signal(SIGINT, handle_ctrl_c);

	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO) && argc == 1)
	{
		/* We are in the terminal, interactive mode */
		errno = 2; /* Setting a custom value for clarity */
		custom_prompt = PROMPT_MSG;
	}
	errno = 0;
	sisifo(custom_prompt, data);
	return (0);
}

/**
 * handle_ctrl_c - Handles the SIGINT signal (Ctrl+C).
 * @opr: Unused parameter for signal handler prototype.
 */
void handle_ctrl_c(int opr UNUSED)
{
	_print("\n");
	_print(PROMPT_MSG);
}

/**
 * initialize_program_data - Initializes the program's data structure.
 * @data: Pointer to the program's data structure.
 * @argv: Array of arguments passed to the program.
 * @env: Environment variables.
 * @argc: Number of values received from the command line.
 */
void initialize_program_data(data_of_program *data, int argc, char *argv[], char **env)
{
	int index = 0;

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
			_print_error_message(data->program_name, argv[1]);
			exit(127);
		}
	}

	data->tokens = NULL;
	data->env = malloc(sizeof(char *) * 50);

	if (env)
	{
		while (env[index])
		{
			data->env[index] = str_duplicate(env[index]);
			index++;
		}
	}
	data->env[index] = NULL;
	env = data->env;

	data->alias_list = malloc(sizeof(char *) * 20);

	for (index = 0; index < 20; index++)
	{
		data->alias_list[index] = NULL;
	}
}

/**
 * sisifo - Infinite loop to display the prompt and handle user input.
 * @prompt: Custom prompt to be printed.
 * @data: Pointer to the program's data structure.
 */
void sisifo(char *prompt, data_of_program *data)
{
	int error_code = 0, string_len = 0;

	while (++(data->exec_counter))
	{
		_print(prompt);
		error_code = string_len = _custom_getline(data);

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

#include "shell.h"

/**
 * @brief Exit the shell.
 *
 * Exits the shell with a given exit status, 0 if info_struct->argv[0] != "exit".
 *
 * @param info_struct Structure containing potential arguments.
 * @return Exits with the specified exit status.
 */
int exit_shell(info_t *info_struct)
{
	int exit_status;

	if (info_struct->argv[1])  /* Check if there is an exit argument */
	{
		exit_status = _erratoi(info_struct->argv[1]);
		if (exit_status == -1)
		{
			info_struct->status = 2;
			print_error(info_struct, "Illegal number: ");
			_eputs(info_struct->argv[1]);
			_eputchar('\n');
			return (1);
		}
		info_struct->err_num = _erratoi(info_struct->argv[1]);
		return (-2);
	}
	info_struct->err_num = -1;
	return (-2);
}

/**
 * @brief Change the current directory of the process.
 *
 * Changes the current directory of the process based on the provided arguments.
 *
 * @param info_struct Structure containing potential arguments.
 * @return Always returns 0.
 */
int change_directory(info_t *info_struct)
{
	char *current_dir, *target_dir, buffer[1024];
	int chdir_result;

	current_dir = getcwd(buffer, 1024);
	if (!current_dir)
		_puts("TODO: Handle getcwd failure message here\n");

	if (!info_struct->argv[1])
	{
		target_dir = _getenv(info_struct, "HOME=");
		if (!target_dir)
			chdir_result = /* TODO: Handle this case appropriately */
				chdir((target_dir = _getenv(info_struct, "PWD=")) ? target_dir : "/");
		else
			chdir_result = chdir(target_dir);
	}
	else if (_strcmp(info_struct->argv[1], "-") == 0)
	{
		if (!_getenv(info_struct, "OLDPWD="))
		{
			_puts(current_dir);
			_putchar('\n');
			return (1);
		}
		_puts(_getenv(info_struct, "OLDPWD=")), _putchar('\n');
		chdir_result = /* TODO: Handle this case appropriately */
			chdir((target_dir = _getenv(info_struct, "OLDPWD=")) ? target_dir : "/");
	}
	else
		chdir_result = chdir(info_struct->argv[1]);

	if (chdir_result == -1)
	{
		print_error(info_struct, "can't cd to ");
		_eputs(info_struct->argv[1]), _eputchar('\n');
	}
	else
	{
		_setenv(info_struct, "OLDPWD", _getenv(info_struct, "PWD="));
		_setenv(info_struct, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * @brief Display help information.
 *
 * Displays help information based on the provided arguments.
 *
 * @param info_struct Structure containing potential arguments.
 * @return Always returns 0.
 */
int display_help(info_t *info_struct)
{
	char **arg_array;

	arg_array = info_struct->argv;
	_puts("Help call works. Function not yet implemented \n");

	/* Temporary workaround to avoid unused variable warning */
	if (0)
		_puts(*arg_array);

	return (0);
}

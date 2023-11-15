#include "shell.h"

/**
 * print_alias - add, remove or show aliases
 * @data: struct for the program's data
 * @alias: name of the alias to be printed
 * Return: zero if sucess, or other number if its declared in the arguments
 */
int print_alias(data_of_program *data, char *alias)
{
int i, j, alias_length;
char buffer[250] = {'\0'};

if (data->alias_list)
{
alias_length = str_length(alias);
for (i = 0; data->alias_list[i]; i++)
{
if (!alias || (str_compare(data->alias_list[i], alias, alias_length)
&&	data->alias_list[i][alias_length] == '='))
{
for (j = 0; data->alias_list[i][j]; j++)
{
buffer[j] = data->alias_list[i][j];
if (data->alias_list[i][j] == '=')
break;
}
buffer[j + 1] = '\0';
buffer_add(buffer, "'");
buffer_add(buffer, data->alias_list[i] + j + 1);
buffer_add(buffer, "'\n");
_print(buffer);
}
}
}

return (0);
}

/**
* get_alias - add, remove or show aliases
* @data: struct for the program's data
* @name: name of the requested alias.
* Return: zero if sucess, or other number if its declared in the arguments
*/
char *get_alias(data_of_program *data, char *name)
{
int i, alias_length;

/* validate the arguments */
if (name == NULL || data->alias_list == NULL)
return (NULL);

alias_length = str_length(name);

for (i = 0; data->alias_list[i]; i++)
{/* Iterates through the environ and check for coincidence of the varname */
if (str_compare(name, data->alias_list[i], alias_length) &&
data->alias_list[i][alias_length] == '=')
{/* returns the value of the key NAME=  when find it */
return (data->alias_list[i] + alias_length + 1);
}
}
/* returns NULL if did not find it */
return (NULL);

}

/**
* set_alias - add, or override alias
* @alias_string: alias to be seted in the form (name='value')
* @data: struct for the program's data
* Return: zero if sucess, or other number if its declared in the arguments
*/
int set_alias(char *alias_string, data_of_program *data)
{
int i, j;
char buffer[250] = {'0'}, *temp = NULL;

/* validate the arguments */
if (alias_string == NULL ||  data->alias_list == NULL)
return (1);
/* Iterates alias to find = char */
for (i = 0; alias_string[i]; i++)
if (alias_string[i] != '=')
buffer[i] = alias_string[i];
else
{/* search if the value of the alias is another alias */
temp = get_alias(data, alias_string + i + 1);
break;
}

/* Iterates through the alias list and check for coincidence of the varname */
for (j = 0; data->alias_list[j]; j++)
if (str_compare(buffer, data->alias_list[j], i) &&
data->alias_list[j][i] == '=')
{/* if the alias alredy exist */
free(data->alias_list[j]);
break;
}

/* add the alias */
if (temp)
{/* if the alias already exist */
buffer_add(buffer, "=");
buffer_add(buffer, temp);
data->alias_list[j] = str_duplicate(buffer);
}
else /* if the alias does not exist */
data->alias_list[j] = str_duplicate(alias_string);
return (0);
}

#include "shell.h"

/**
* builtin_env - shows the environment where the shell runs
* @data: struct for the program's data
* Return: zero if sucess, or other number if its declared in the arguments
*/
int builtin_env(data_of_program *data)
{
int i;
char cpname[50] = {'\0'};
char *var_copy = NULL;

/* if not arguments */
if (data->tokens[1] == NULL)
print_environ(data);
else
{
for (i = 0; data->tokens[1][i]; i++)
{/* checks if exists a char = */
if (data->tokens[1][i] == '=')
{/* checks if exists a var with the same name and change its value*/
/* temporally */
var_copy = str_duplicate(env_get_key(cpname, data));
if (var_copy != NULL)
env_set_key(cpname, data->tokens[1] + i + 1, data);

/* print the environ */
print_environ(data);
if (env_get_key(cpname, data) == NULL)
{/* print the variable if it does not exist in the environ */
_print(data->tokens[1]);
_print("\n");
}
else
{/* returns the old value of the var*/
env_set_key(cpname, var_copy, data);
free(var_copy);
}
return (0);
}
cpname[i] = data->tokens[1][i];
}
errno = 2;
perror(data->command_name);
errno = 127;
}
return (0);
}

/**
* builtin_set_env - ..
* @data: struct for the program's data
* Return: zero if sucess, or other number if its declared in the arguments
*/
int builtin_set_env(data_of_program *data)
{
/* validate args */
if (data->tokens[1] == NULL || data->tokens[2] == NULL)
return (0);
if (data->tokens[3] != NULL)
{
errno = E2BIG;
perror(data->command_name);
return (5);
}

env_set_key(data->tokens[1], data->tokens[2], data);

return (0);
}

/**
* builtin_unset_env - ..
* @data: struct for the program's data'
* Return: ..
*/
int builtin_unset_env(data_of_program *data)
{
/* validate args */
if (data->tokens[1] == NULL)
return (0);
if (data->tokens[2] != NULL)
{
errno = E2BIG;
perror(data->command_name);
return (5);
}
env_remove_key(data->tokens[1], data);

return (0);
}

#include "shell.h"

/**
* builtins_list - search for match and execute the associate builtin
* @data: struct for the program's data
* Return: Returns the return of the function executed is there is a match,
* otherwise returns -1.
**/
int builtins_list(data_of_program *data)
{
int iterator;
builtins options[] = {
{"exit", builtin_exit},
{"help", builtin_help},
{"cd", builtin_cd},
{"alias", builtin_alias},
{"env", builtin_env},
{"setenv", builtin_set_env},
{"unsetenv", builtin_unset_env},
{NULL, NULL}
};

for (iterator = 0; options[iterator].builtin != NULL; iterator++)
{
if (str_compare(options[iterator].builtin, data->command_name, 0))
{
return (options[iterator].function(data));
}
}
return (-1);
}

#include "shell.h"

/**
* builtin_exit - exit of the program with the status
* @data: struct for the program's data
* Return: zero if sucess, or other number if its declared in the arguments
*/
int builtin_exit(data_of_program *data)
{
int i;

if (data->tokens[1] != NULL)
{
for (i = 0; data->tokens[1][i]; i++)
if ((data->tokens[1][i] < '0' || data->tokens[1][i] > '9')
&& data->tokens[1][i] != '+')
{/*if is not a number*/
errno = 2;
return (2);
}
errno = _atoi(data->tokens[1]);
}
free_all_data(data);
exit(errno);
}

/**
* builtin_cd - change the current directory
* @data: struct for the program's data
* Return: zero if sucess, or other number if its declared in the arguments
*/
int builtin_cd(data_of_program *data)
{
char *dir_home = env_get_key("HOME", data), *dir_old = NULL;
char old_dir[128] = {0};
int error_code = 0;

if (data->tokens[1])
{
if (str_compare(data->tokens[1], "-", 0))
{
dir_old = env_get_key("OLDPWD", data);
if (dir_old)
error_code = set_work_directory(data, dir_old);
_print(env_get_key("PWD", data));
_print("\n");

return (error_code);
}
else
{
return (set_work_directory(data, data->tokens[1]));
}
}
else
{
if (!dir_home)
dir_home = getcwd(old_dir, 128);

return (set_work_directory(data, dir_home));
}
return (0);
}

/**
* set_work_directory - set the work directory
* @data: struct for the program's data
* @new_dir: path to be set as work directory
* Return: zero if sucess, or other number if its declared in the arguments
*/
int set_work_directory(data_of_program *data, char *new_dir)
{
char old_dir[128] = {0};
int err_code = 0;

getcwd(old_dir, 128);

if (!str_compare(old_dir, new_dir, 0))
{
err_code = chdir(new_dir);
if (err_code == -1)
{
errno = 2;
return (3);
}
env_set_key("PWD", new_dir, data);
}
env_set_key("OLDPWD", old_dir, data);
return (0);
}

/**
* builtin_help - shows the environment where the shell runs
* @data: struct for the program's data
* Return: zero if sucess, or other number if its declared in the arguments
*/
int builtin_help(data_of_program *data)
{
int i, length = 0;
char *mensajes[6] = {NULL};

mensajes[0] = HELP_MSG;

if (data->tokens[1] == NULL)
{
_print(mensajes[0] + 6);
return (1);
}
if (data->tokens[2] != NULL)
{
errno = E2BIG;
perror(data->command_name);
return (5);
}
mensajes[1] = HELP_EXIT_MSG;
mensajes[2] = HELP_ENV_MSG;
mensajes[3] = HELP_SETENV_MSG;
mensajes[4] = HELP_UNSETENV_MSG;
mensajes[5] = HELP_CD_MSG;

for (i = 0; mensajes[i]; i++)
{
length = str_length(data->tokens[1]);
if (str_compare(data->tokens[1], mensajes[i], length))
{
_print(mensajes[i] + length + 1);
return (1);
}
}
errno = EINVAL;
perror(data->command_name);
return (0);
}

/**
* builtin_alias - add, remove or show aliases
* @data: struct for the program's data
* Return: zero if sucess, or other number if its declared in the arguments
*/
int builtin_alias(data_of_program *data)
{
int i = 0;

if (data->tokens[1] == NULL)
return (print_alias(data, NULL));

while (data->tokens[++i])
{
if (count_characters(data->tokens[i], "="))
set_alias(data->tokens[i], data);
else
print_alias(data, data->tokens[i]);
}

return (0);
}

/*Authors Waltertaya and BarukNetizen*/

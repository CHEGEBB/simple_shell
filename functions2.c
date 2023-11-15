#include "shell.h"

/**
 * env_get_key - gets the value of an environment variable
 * @key: the environment variable of interest
 * @data: struct of the program's data
 * Return: a pointer to the value of the variable or NULL if it doesn't exist
 */
char *env_get_key(char *key, data_of_program *data)
{
int i, key_length = 0;

if (key == NULL || data->env == NULL)
return (NULL);

key_length = str_length(key);

for (i = 0; data->env[i]; i++)
{
if (str_compare(key, data->env[i], key_length) &&
data->env[i][key_length] == '=')
{
return (data->env[i] + key_length + 1);
}
}
return (NULL);
}

/**
 * env_set_key - overwrite the value of the environment variable
 * or create it if does not exist.
 * @key: name of the variable to set
 * @value: new value
 * @data: struct of the program's data
 * Return: 1 if the parameters are NULL, 2 if there is an erroror 0 if sucess.
 */

int env_set_key(char *key, char *value, data_of_program *data)
{
int i, key_length = 0, is_new_key = 1;

if (key == NULL || value == NULL || data->env == NULL)
return (1);

key_length = str_length(key);

for (i = 0; data->env[i]; i++)
{
if (str_compare(key, data->env[i], key_length) &&
data->env[i][key_length] == '=')
{
is_new_key = 0;
free(data->env[i]);
break;
}
}
data->env[i] = str_concat(str_duplicate(key), "=");
data->env[i] = str_concat(data->env[i], value);

if (is_new_key)
{
data->env[i + 1] = NULL;
}
return (0);
}

/**
 * env_remove_key - remove a key from the environment
 * @key: the key to remove
 * @data: the sructure of the program's data
 * Return: 1 if the key was removed, 0 if the key does not exist;
 */
int env_remove_key(char *key, data_of_program *data)
{
int i, key_length = 0;

if (key == NULL || data->env == NULL)
return (0);

key_length = str_length(key);

for (i = 0; data->env[i]; i++)
{
if (str_compare(key, data->env[i], key_length) &&
data->env[i][key_length] == '=')
{
free(data->env[i]);

i++;
for (; data->env[i]; i++)
{
data->env[i - 1] = data->env[i];
}
data->env[i - 1] = NULL;
return (1);
}
}
return (0);
}


/**
 * print_environ - prints the current environ
 * @data: struct for the program's data
 * Return: nothing
 */
void print_environ(data_of_program *data)
{
int j;

for (j = 0; data->env[j]; j++)
{
_print(data->env[j]);
_print("\n");
}
}

#include "shell.h"
/**
 * execute - execute a command with its entire path variables.
 * @data: a pointer to the program's data
 * Return: If sucess returns zero, otherwise, return -1.
 */
int execute(data_of_program *data)
{
int retval = 0, status;
pid_t pidd;

retval = builtins_list(data);
if (retval != -1)
return (retval);

retval = find_program(data);
if (retval)
{
return (retval);
}
else
{
pidd = fork();
if (pidd == -1)
{
perror(data->command_name);
exit(EXIT_FAILURE);
}
if (pidd == 0)
{
retval = execve(data->tokens[0], data->tokens, data->env);
if (retval == -1) /* if error when execve*/
perror(data->command_name), exit(EXIT_FAILURE);
}
else
{
wait(&status);
if (WIFEXITED(status))
errno = WEXITSTATUS(status);
else if (WIFSIGNALED(status))
errno = 128 + WTERMSIG(status);
}
}
return (0);
}

#include "shell.h"

/**
 * expand_variables - expand variables
 * @data: a pointer to a struct of the program's data
 *
 * Return: nothing, but sets errno.
 */
void expand_variables(data_of_program *data)
{
int i, j;
char line[BUFFER_SIZE] = {0}, expansion[BUFFER_SIZE] = {'\0'}, *temp;

if (data->input_line == NULL)
return;
buffer_add(line, data->input_line);
for (i = 0; line[i]; i++)
if (line[i] == '#')
line[i--] = '\0';
else if (line[i] == '$' && line[i + 1] == '?')
{
line[i] = '\0';
long_to_string(errno, expansion, 10);
buffer_add(line, expansion);
buffer_add(line, data->input_line + i + 2);
}
else if (line[i] == '$' && line[i + 1] == '$')
{
line[i] = '\0';
long_to_string(getpid(), expansion, 10);
buffer_add(line, expansion);
buffer_add(line, data->input_line + i + 2);
}
else if (line[i] == '$' && (line[i + 1] == ' ' || line[i + 1] == '\0'))
continue;
else if (line[i] == '$')
{
for (j = 1; line[i + j] && line[i + j] != ' '; j++)
expansion[j - 1] = line[i + j];
temp = env_get_key(expansion, data);
line[i] = '\0', expansion[0] = '\0';
buffer_add(expansion, line + i + j);
temp ? buffer_add(line, temp) : 1;
buffer_add(line, expansion);
}
if (!str_compare(data->input_line, line, 0))
{
free(data->input_line);
data->input_line = str_duplicate(line);
}
}

/**
 * expand_alias - expans aliases
 * @data: a pointer to a struct of the program's data
 *
 * Return: nothing, but sets errno.
 */
void expand_alias(data_of_program *data)
{
int i, j, was_expanded = 0;
char line[BUFFER_SIZE] = {0}, expansion[BUFFER_SIZE] = {'\0'}, *temp;

if (data->input_line == NULL)
return;

buffer_add(line, data->input_line);

for (i = 0; line[i]; i++)
{
for (j = 0; line[i + j] && line[i + j] != ' '; j++)
expansion[j] = line[i + j];
expansion[j] = '\0';

temp = get_alias(data, expansion);
if (temp)
{
expansion[0] = '\0';
buffer_add(expansion, line + i + j);
line[i] = '\0';
buffer_add(line, temp);
line[str_length(line)] = '\0';
buffer_add(line, expansion);
was_expanded = 1;
}
break;
}
if (was_expanded)
{
free(data->input_line);
data->input_line = str_duplicate(line);
}
}

/**
 * buffer_add - append string at end of the buffer
 * @buffer: buffer to be filled
 * @str_to_add: string to be copied in the buffer
 * Return: nothing, but sets errno.
 */
int buffer_add(char *buffer, char *str_to_add)
{
int length, i;

length = str_length(buffer);
for (i = 0; str_to_add[i]; i++)
{
buffer[length + i] = str_to_add[i];
}
buffer[length + i] = '\0';
return (length + i);
}

#include "shell.h"

int check_file(char *full_path);

/**
* find_program - find a program in path
* @data: a pointer to the program's data
* Return: 0 if success, errcode otherwise
*/

int find_program(data_of_program *data)
{
int i = 0, ret_code = 0;
char **directories;

if (!data->command_name)
return (2);

if (data->command_name[0] == '/' || data->command_name[0] == '.')
return (check_file(data->command_name));

free(data->tokens[0]);
data->tokens[0] = str_concat(str_duplicate("/"), data->command_name);
if (!data->tokens[0])
return (2);

directories = tokenize_path(data);/* search in the PATH */

if (!directories || !directories[0])
{
errno = 127;
return (127);
}
for (i = 0; directories[i]; i++)
{/* appends the function_name to path */
directories[i] = str_concat(directories[i], data->tokens[0]);
ret_code = check_file(directories[i]);
if (ret_code == 0 || ret_code == 126)
{/* the file was found, is not a directory and has execute permisions*/
errno = 0;
free(data->tokens[0]);
data->tokens[0] = str_duplicate(directories[i]);
free_array_of_pointers(directories);
return (ret_code);
}
}
free(data->tokens[0]);
data->tokens[0] = NULL;
free_array_of_pointers(directories);
return (ret_code);
}

/**
* tokenize_path - tokenize the path in directories
* @data: a pointer to the program's data
* Return: array of path directories
*/

char **tokenize_path(data_of_program *data)
{
int i = 0;
int counter_directories = 2;
char **tokens = NULL;
char *PATH;

PATH = env_get_key("PATH", data);
if ((PATH == NULL) || PATH[0] == '\0')
{
return (NULL);
}

PATH = str_duplicate(PATH);

for (i = 0; PATH[i]; i++)
{
if (PATH[i] == ':')
counter_directories++;
}

tokens = malloc(sizeof(char *) * counter_directories);

i = 0;
tokens[i] = str_duplicate(_strtok(PATH, ":"));
while (tokens[i++])
{
tokens[i] = str_duplicate(_strtok(NULL, ":"));
}

free(PATH);
PATH = NULL;
return (tokens);

}

/**
* check_file - checks if exists a file, if it is not a dairectory and
* if it has excecution permisions for permisions.
* @full_path: pointer to the full file name
* Return: 0 on success, or error code if it exists.
*/

int check_file(char *full_path)
{
struct stat sb;

if (stat(full_path, &sb) != -1)
{
if (S_ISDIR(sb.st_mode) ||  access(full_path, X_OK))
{
errno = 126;
return (126);
}
return (0);
}
errno = 127;
return (127);
}

/*Authors waltertaya and BarukNetizen*/

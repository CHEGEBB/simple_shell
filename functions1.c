#include "shell.h"

/**
* _getline - read one line from the prompt.
* @data: struct for the program's data
*
* Return: reading counting bytes.
*/
int _getline(data_of_program *data)
{
char buff[BUFFER_SIZE] = {'\0'};
static char *array_commands[10] = {NULL};
static char array_operators[10] = {'\0'};
ssize_t bytes_read, i = 0;

if (!array_commands[0] || (array_operators[0] == '&' && errno != 0) ||
(array_operators[0] == '|' && errno == 0))
{
/*free the memory allocated in the array if it exists */
for (i = 0; array_commands[i]; i++)
{
free(array_commands[i]);
array_commands[i] = NULL;
}

bytes_read = read(data->file_descriptor, &buff, BUFFER_SIZE - 1);
if (bytes_read == 0)
return (-1);

i = 0;
do {
array_commands[i] = str_duplicate(_strtok(i ? NULL : buff, "\n;"));
i = check_logic_ops(array_commands, i, array_operators);
} while (array_commands[i++]);
}

data->input_line = array_commands[0];
for (i = 0; array_commands[i]; i++)
{
array_commands[i] = array_commands[i + 1];
array_operators[i] = array_operators[i + 1];
}

return (str_length(data->input_line));
}


/**
* check_logic_ops - checks and split for && and || operators
* @array_commands: array of the commands.
* @i: index in the array_commands to be checked
* @array_operators: array of the logical operators for each previous command
*
* Return: index of the last command in the array_commands.
*/
int check_logic_ops(char *array_commands[], int i, char array_operators[])
{
char *temp = NULL;
int j;

for (j = 0; array_commands[i] != NULL  && array_commands[i][j]; j++)
{
if (array_commands[i][j] == '&' && array_commands[i][j + 1] == '&')
{
temp = array_commands[i];
array_commands[i][j] = '\0';
array_commands[i] = str_duplicate(array_commands[i]);
array_commands[i + 1] = str_duplicate(temp + j + 2);
i++;
array_operators[i] = '&';
free(temp);
j = 0;
}
if (array_commands[i][j] == '|' && array_commands[i][j + 1] == '|')
{
temp = array_commands[i];
array_commands[i][j] = '\0';
array_commands[i] = str_duplicate(array_commands[i]);
array_commands[i + 1] = str_duplicate(temp + j + 2);
i++;
array_operators[i] = '|';
free(temp);
j = 0;
}
}
return (i);
}

#include "shell.h"
/**
 * _strtok - separates strings with delimiters
 * @line: It´s pointer to array we receive in getline.
 * @delim: It´s characters we mark off string in parts.
 * Return: A pointer to the created token
*/
char *_strtok(char *line, char *delim)
{
int j;
static char *str;
char *copystr;

if (line != NULL)
str = line;
for (; *str != '\0'; str++)
{
for (j = 0; delim[j] != '\0'; j++)
{
if (*str == delim[j])
break;
}
if (delim[j] == '\0')
break;
}
copystr = str;
if (*copystr == '\0')
return (NULL);
for (; *str != '\0'; str++)
{
for (j = 0; delim[j] != '\0'; j++)
{
if (*str == delim[j])
{
*str = '\0';
str++;
return (copystr);
}
}
}
return (copystr);
}

#include "shell.h"
/**
 * tokenize - this function separate the string using a designed delimiter
 * @data: a pointer to the program's data
 * Return: an array of the different parts of the string
 */
void tokenize(data_of_program *data)
{
char *delimiter = " \t";
int i, j, counter = 2, length;

length = str_length(data->input_line);
if (length)
{
if (data->input_line[length - 1] == '\n')
data->input_line[length - 1] = '\0';
}

for (i = 0; data->input_line[i]; i++)
{
for (j = 0; delimiter[j]; j++)
{
if (data->input_line[i] == delimiter[j])
counter++;
}
}

data->tokens = malloc(counter * sizeof(char *));
if (data->tokens == NULL)
{
perror(data->program_name);
exit(errno);
}
i = 0;
data->tokens[i] = str_duplicate(_strtok(data->input_line, delimiter));
data->command_name = str_duplicate(data->tokens[0]);
while (data->tokens[i++])
{
data->tokens[i] = str_duplicate(_strtok(NULL, delimiter));
}
}

/*Authors Waltertaya and BarukNetizen*/

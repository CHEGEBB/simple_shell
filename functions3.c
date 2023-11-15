#include "shell.h"

/**
 * free_recurrent_data - free the fields needed each loop
 * @data: struct of the program's data
 * Return: Nothing
 */
void free_recurrent_data(data_of_program *data)
{
if (data->tokens)
free_array_of_pointers(data->tokens);
if (data->input_line)
free(data->input_line);
if (data->command_name)
free(data->command_name);

data->input_line = NULL;
data->command_name = NULL;
data->tokens = NULL;
}

/**
 * free_all_data - free all field of the data
 * @data: struct of the program's data
 * Return: Nothing
 */
void free_all_data(data_of_program *data)
{
if (data->file_descriptor != 0)
{
if (close(data->file_descriptor))
perror(data->program_name);
}
free_recurrent_data(data);
free_array_of_pointers(data->env);
free_array_of_pointers(data->alias_list);
}

/**
 * free_array_of_pointers - frees each pointer of an array of pointers and the
 * array too
 * @array: array of pointers
 * Return: nothing
 */
void free_array_of_pointers(char **array)
{
int i;

if (array != NULL)
{
for (i = 0; array[i]; i++)
free(array[i]);

free(array);
array = NULL;
}
}

#include "shell.h"

/**
 * long_to_string - converts a number to a string.
 * @number: number to be converten in an string.
 * @string: buffer to save the number as string.
 * @base: base to convert number
 *
 * Return: Nothing.
 */
void long_to_string(long number, char *string, int base)
{
int index = 0, inNegative = 0;
long cociente = number;
char letters[] = {"0123456789abcdef"};

if (cociente == 0)
string[index++] = '0';

if (string[0] == '-')
inNegative = 1;

while (cociente)
{
if (cociente < 0)
string[index++] = letters[-(cociente % base)];
else
string[index++] = letters[cociente % base];
cociente /= base;
}
if (inNegative)
string[index++] = '-';

string[index] = '\0';
str_reverse(string);
}


/**
 * _atoi - convert a string to an integer.
 *
 * @s: pointer to str origen.
 * Return: int of string or 0.
 */
int _atoi(char *s)
{
int sign = 1;
unsigned int number = 0;
while (!('0' <= *s && *s <= '9') && *s != '\0')
{
if (*s == '-')
sign *= -1;
if (*s == '+')
sign *= +1;
s++;
}

while ('0' <= *s && *s <= '9' && *s != '\0')
{

number = (number * 10) + (*s - '0');
s++;
}
return (number * sign);
}

/**
 * count_characters - count the coincidences of character in string.
 *
 * @string: pointer to str origen.
 * @character: string with  chars to be counted
 * Return: int of string or 0.
 */
int count_characters(char *string, char *character)
{
int i = 0, counter = 0;

for (; string[i]; i++)
{
if (string[i] == character[0])
counter++;
}
return (counter);
}

#include "shell.h"

/**
 * _print - writes a array of chars in the standar output
 * @string: pointer to the array of chars
 * Return: the number of bytes writed or .
 * On error, -1 is returned, and errno is set appropriately.
 */
int _print(char *string)
{
return (write(STDOUT_FILENO, string, str_length(string)));
}
/**
 * _printe - writes a array of chars in the standar error
 * @string: pointer to the array of chars
 * Return: the number of bytes writed or .
 * On error, -1 is returned, and errno is set appropriately.
 */
int _printe(char *string)
{
return (write(STDERR_FILENO, string, str_length(string)));
}

/**
 * _print_error - writes a array of chars in the standart error
 * @data: a pointer to the program's data'
 * @errorcode: error code to print
 * Return: the number of bytes writed or .
 * On error, -1 is returned, and errno is set appropriately.
 */
int _print_error(int errorcode, data_of_program *data)
{
char n_as_string[10] = {'\0'};

long_to_string((long) data->exec_counter, n_as_string, 10);

if (errorcode == 2 || errorcode == 3)
{
_printe(data->program_name);
_printe(": ");
_printe(n_as_string);
_printe(": ");
_printe(data->tokens[0]);
if (errorcode == 2)
_printe(": Illegal number: ");
else
_printe(": can't cd to ");
_printe(data->tokens[1]);
_printe("\n");
}
else if (errorcode == 127)
{
_printe(data->program_name);
_printe(": ");
_printe(n_as_string);
_printe(": ");
_printe(data->command_name);
_printe(": not found\n");
}
else if (errorcode == 126)
{
_printe(data->program_name);
_printe(": ");
_printe(n_as_string);
_printe(": ");
_printe(data->command_name);
_printe(": Permission denied\n");
}
return (0);
}

#include "shell.h"

/**
 * str_length - returns the length of a string.
 * @string: pointer to string.
 * Return: length of string.
 */
int str_length(char *string)
{
int length = 0;

if (string == NULL)
return (0);

while (string[length++] != '\0')
{
}
return (--length);
}

/**
 * str_duplicate - duplicates an string
 * @string: String to be copied
 * Return: pointer to the array
 */
char *str_duplicate(char *string)
{
char *result;
int length, i;

if (string == NULL)
return (NULL);

length = str_length(string) + 1;

result = malloc(sizeof(char) * length);

if (result == NULL)
{
errno = ENOMEM;
perror("Error");
return (NULL);
}
for (i = 0; i < length ; i++)
{
result[i] = string[i];
}

return (result);
}

/**
 * str_compare - Compare two strings
 * @string1: String one, or the shorter
 * @string2: String two, or the longer
 * @number: number of characters to be compared, 0 if infinite
 * Return: 1 if the strings are equals,0 if the strings are different
 */
int str_compare(char *string1, char *string2, int number)
{
int iterator;

if (string1 == NULL && string2 == NULL)
return (1);

if (string1 == NULL || string2 == NULL)
return (0);

if (number == 0)
{
if (str_length(string1) != str_length(string2))
return (0);
for (iterator = 0; string1[iterator]; iterator++)
{
if (string1[iterator] != string2[iterator])
return (0);
}
return (1);
}
else
{
for (iterator = 0; iterator < number ; iterator++)
{
if (string1[iterator] != string2[iterator])
return (0);
}
return (1);
}
}

/**
 * str_concat - concatenates two strings.
 * @string1: String to be concatenated
 * @string2: String to be concatenated
 *
 * Return: pointer to the array
 */
char *str_concat(char *string1, char *string2)
{
char *result;
int length1 = 0, length2 = 0;

if (string1 == NULL)
string1 = "";
length1 = str_length(string1);

if (string2 == NULL)
string2 = "";
length2 = str_length(string2);

result = malloc(sizeof(char) * (length1 + length2 + 1));
if (result == NULL)
{
errno = ENOMEM;
perror("Error");
return (NULL);
}

for (length1 = 0; string1[length1] != '\0'; length1++)
result[length1] = string1[length1];
free(string1);

for (length2 = 0; string2[length2] != '\0'; length2++)
{
result[length1] = string2[length2];
length1++;
}

result[length1] = '\0';
return (result);
}


/**
 * str_reverse - reverses a string.
 *
 * @string: pointer to string.
 * Return: void.
 */
void str_reverse(char *string)
{

int i = 0, length = str_length(string) - 1;
char hold;

while (i < length)
{
hold = string[i];
string[i++] = string[length];
string[length--] = hold;
}
}
/*Authors Waltertaya and BarukNetizen*/
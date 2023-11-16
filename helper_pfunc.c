#include "shell.h"

/**
 * print_string: Writes a string to the standard output.
 * @param string: The string to print.
 * @return: The number of bytes written, or -1 on error.
 */
int print_string(char *string) {
  return write(STDOUT_FILENO, string, strlen(string));
}

/**
 * print_error: Prints an error message to the standard error output.
 * @param error_code: The error code to print.
 * @param data: The program data.
 * @return: 0 on success, or -1 on error.
 */
int print_error(int error_code, data_of_program *data) {
  char execution_counter_string[10] = {'\0'};

  convert_long_to_string((long) data->execution_counter, execution_counter_string, 10);

  if (error_code == 2 || error_code == 3) {
    print_string(data->program_name);
    print_string(": ");
    print_string(execution_counter_string);
    print_string(": ");
    print_string(data->tokens[0]);

    if (error_code == 2) {
      print_string(": Invalid number: ");
    } else {
      print_string(": Unable to change directory to ");
    }

    print_string(data->tokens[1]);
    print_string("\n");
  } else if (error_code == 127) {
    print_string(data->program_name);
    print_string(": ");
    print_string(execution_counter_string);
    print_string(": ");
    print_string(data->command_name);
    print_string(": Command not found\n");
  } else if (error_code == 126) {
    print_string(data->program_name);
    print_string(": ");
    print_string(execution_counter_string);
    print_string(": ");
    print_string(data->command_name);
    print_string(": Permission denied\n");
  }

  return 0;
}

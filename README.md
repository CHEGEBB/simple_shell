# 0x16C Simple Shell Project
Simple Shell Project, which is a group project for students at ALX SE. The goal of the project is to create a simple shell that can execute basic commands.

Writing a simple shell can be a great learning experience for anyone interested in operating systems and programming. In this project, we will go through the process of making a simple shell using C language.

### What is a shell?

A shell is a program that acts as an interface between the user and the operating system. A shell allows the user to interact with the operating system by accepting commands entered by the user from the keyboard, verifying if the inputs from the user are correct and executing them. Generally, a shell is a user interface that allows us to use computer resources such as memory. It can take two forms command-line interface such as Bash or graphical user interface such as Windows Office.

### Authors
a) `@waltertaya`
b) `@Baruk1-netizen`


## Requirements

- The shell must be written in C.
- The shell must use the Betty style.
- The shell must not have any memory leaks.
- The shell must use system calls only when necessary.
- The shell must be compiled with the following options: `-Wall -Werror -Wextra -pedantic -std=gnu89`.
- The shell must have a `README.md` file and an `AUTHORS` file.
- The shell must have the same output as `sh` (`/bin/sh`), except for errors, which must be prefixed with the name of the program (e.g., `./hsh: 1: qwerty: not found`).

## Allowed Functions and System Calls

- `access`
- `chdir`
- `close`
- `closedir`
- `execve`
- `exit`
- `_exit`
- `fflush`
- `fork`
- `free`
- `getcwd`
- `getline`
- `getpid`
- `isatty`
- `kill`
- `malloc`
- `open`
- `opendir`
- `perror`
- `read`
- `readdir`
- `signal`
- `stat` (also `__xstat`, `__lxstat`, and `__fxstat`)
- `strtok`
- `wait`
- `waitpid`
- `wait3`
- `wait4`
- `write`

## Testing

The shell will be tested in both interactive and non-interactive modes.

### Interactive Mode

The shell should prompt the user for input and execute the commands that are entered.

```shell
waltermitty@waltermitty-VivoBook-15-ASUS-Laptop-X540UAR:~/shell$ gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
waltermitty@waltermitty-VivoBook-15-ASUS-Laptop-X540UAR:~/shell$ ./hsh
$ls
Attempt  AUTHORS  functions1.c	functions2.c  functions3.c  functions.c  helper_macros.h  hsh  main.c  manual_simple_shell  README.md  shell.h
$pwd
/home/waltermitty/shell
$




#ifndef HELPERS_H
#define HELPERS_H

/* PROMPT_MSG - prompt message for the shell @PROMPT_MSG: prompt message for the shell */

#define PROMPT_MSG "$"

/* unused */

#define UNUSED __attribute__((unused))

/* Buffer size*/

#define BUFFER_SIZE 1024

/* HELP_CD_MSG - help message for cd builtin @HELP_CD_MSG: help message for cd builtin */
 

#define HELP_CD_MSG "cd=\n"\
"cd:\tcd [dir]\n\n"\
"	Change the shell working directory.\n\n"\
"	If no argument is given to cd the command will be interpreted\n"\
"	as cd $HOME.\n"\
"	if the argumenthelp is - (cd -), the command will be interpreted\n"\
"	as cd $OLDPWD.\n\n"

/* HELP_EXIT_MSG - help message for exit builtin @HELP_EXIT_MSG: help message for exit builtin */

#define HELP_EXIT_MSG "exit=\n"\
"exit:\texit [STATUS]\n\n"\
"	Exit of the simple-shell.\n\n"\
"	Exits the shell with a status of N.  If N is omitted, the exit status\n"\
"	is that of the last command executed.\n\n"

#define HELP_ENV_MSG "env=\n"\
"env:\tenv \n\n"\
"	Print environment.\n\n"\
"	The env command will be print a complete list of enviroment variables.\n\n"

#define HELP_SETENV_MSG "setenv=\n"\
"setenv:\tsetenv VARIABLE VALUE\n\n"\
"	Change or add an environment variable.\n\n"\
"	initialize a new environment variable, or modify an existing one\n"\
"	When there are not correct numbers of arguments print error message.\n\n"

#define HELP_UNSETENV_MSG "unsetenv=\n"\
"unsetenv:\tunsetenv VARIABLE\n\n"\
"	The unsetenv function deletes one variable from the environment.\n\n"\
"	Wen there are not correct numbers of arguments print error message.\n\n"

#define HELP_MSG "help=\n"\
"help:\thelp [BUILTIN_NAME]\n\n"\
"	Display information about builtin commands.\n\n"\
"	Displays brief summaries of builtin commands.  If BUILTIN_NAME is\n"\
"	specified, gives detailed help on all commands matching BUILTIN_NAME,\n"\
"	otherwise the list of help topics is printed BUILTIN_NAME list.\n"\
"	Arguments:\n\n"\
"	BUILTIN_NAME specifiying a help topic.\n\n"\
"	cd\t[dir]\n"\
"	exit\t[status]\n"\
"	env\n"\
"	setenv\t[variable value]\n"\
"	unset\t[variable]\n"\
"	help\t[built_name]\n\n"

#endif

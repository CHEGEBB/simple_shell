#ifndef HELPERS_H
#define HELPERS_H

/* Prompt to be printed */
#define PROMPT_MSG "$" /* Needed for signal compatibility */

/* Resume from the unused attribute */
#define UNUSED __attribute__((unused))

/* buffer size for each read call in _getline */
#define BUFFER_SIZE 1024

/************* FORMATTED STRINGS FOR BUILT-IN COMMANDS **************/

#define HELP_CD_MSG "cd=\n"\
"cd:\tcd [dir]\n\n"\
"	Change the shell working directory.\n\n"\
"	Omitting arguments defaults to cd $HOME.\n"\
"	Use 'cd -' to interpret the command as cd $OLDPWD.\n\n"

#define HELP_EXIT_MSG "exit=\n"\
"exit:\texit [STATUS]\n\n"\
"	Exit the simple shell.\n\n"\
"	Provide a status (N) to exit with, else uses the status of the last command.\n\n"

#define HELP_ENV_MSG "env=\n"\
"env:\tenv \n\n"\
"	Display environment variables.\n\n"\
"	Use 'env' to print the complete list of environment variables.\n\n"

#define HELP_SETENV_MSG "setenv=\n"\
"setenv:\tsetenv VARIABLE VALUE\n\n"\
"	Change or add an environment variable.\n\n"\
"	Initializing or modifying environment variables.\n"\
"	Display an error if the argument count is incorrect.\n\n"

#define HELP_UNSETENV_MSG "unsetenv=\n"\
"unsetenv:\tunsetenv VARIABLE\n\n"\
"	Delete a variable from the environment.\n\n"\
"	Display an error if the argument count is incorrect.\n\n"

#define HELP_MSG "help=\n"\
"help:\thelp [BUILTIN_NAME]\n\n"\
"	Display information about builtin commands.\n\n"\
"	Summarize or provide detailed help on builtin commands.\n"\
"	When BUILTIN_NAME specified, show help for the matched command,\n"\
"	otherwise, list available help topics.\n"\
"	Arguments:\n\n"\
"	BUILTIN_NAME specifying a help topic.\n\n"\
"	cd\t[dir]\n"\
"	exit\t[status]\n"\
"	env\n"\
"	setenv\t[variable value]\n"\
"	unsetenv\t[variable]\n"\
"	help\t[built_name]\n\n"

#endif

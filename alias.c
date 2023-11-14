#include "shell.h"

/**
 * displayAlias - handle addition, removal, or display of aliases
 * @programData: structure containing program data
 * @requestedAlias: name of the alias to be displayed
 * Return: 0 if successful, otherwise returns an error code
 */
int displayAlias(ProgramData *programData, char *requestedAlias)
{
    int i, j, aliasLength;
    char buffer[250] = {'\0'};

    if (programData->aliases)
    {
        aliasLength = stringLength(requestedAlias);
        for (i = 0; programData->aliases[i]; i++)
        {
            if (!requestedAlias || (stringCompare(programData->aliases[i], requestedAlias, aliasLength)
                && programData->aliases[i][aliasLength] == '='))
            {
                for (j = 0; programData->aliases[i][j]; j++)
                {
                    buffer[j] = programData->aliases[i][j];
                    if (programData->aliases[i][j] == '=')
                        break;
                }
                buffer[j + 1] = '\0';
                appendString(buffer, "'");
                appendString(buffer, programData->aliases[i] + j + 1);
                appendString(buffer, "'\n");
                printString(buffer);
            }
        }
    }

    return 0;
}

/**
 * retrieveAlias - retrieve the value of a specified alias
 * @programData: structure containing program data
 * @name: name of the requested alias.
 * Return: the value of the alias if found, otherwise returns NULL
 */
char *retrieveAlias(ProgramData *programData, char *name)
{
    int i, aliasLength;

    /* validate the arguments */
    if (name == NULL || programData->aliases == NULL)
        return NULL;

    aliasLength = stringLength(name);

    for (i = 0; programData->aliases[i]; i++)
    {
        if (stringCompare(name, programData->aliases[i], aliasLength) &&
            programData->aliases[i][aliasLength] == '=')
        {
            return programData->aliases[i] + aliasLength + 1;
        }
    }

    return NULL;
}

/**
 * setAlias - add or override an alias
 * @aliasString: alias to be set in the form (name='value')
 * @programData: structure containing program data
 * Return: 0 if successful, otherwise returns an error code
 */
int setAlias(char *aliasString, ProgramData *programData)
{
    int i, j;
    char buffer[250] = {'0'}, *temp = NULL;

    /* validate the arguments */
    if (aliasString == NULL ||  programData->aliases == NULL)
        return 1;

    /* Iterates alias to find = char */
    for (i = 0; aliasString[i]; i++)
    {
        if (aliasString[i] != '=')
            buffer[i] = aliasString[i];
        else
        {
            temp = retrieveAlias(programData, aliasString + i + 1);
            break;
        }
    }

    /* Iterates through the alias list and check for coincidence of the varname */
    for (j = 0; programData->aliases[j]; j++)
    {
        if (stringCompare(buffer, programData->aliases[j], i) &&
            programData->aliases[j][i] == '=')
        {
            free(programData->aliases[j]);
            break;
        }
    }

    /* add the alias */
    if (temp)
    {
        appendString(buffer, "=");
        appendString(buffer, temp);
        programData->aliases[j] = stringDuplicate(buffer);
    }
    else /* if the alias does not exist */
        programData->aliases[j] = stringDuplicate(aliasString);

    return 0;
}

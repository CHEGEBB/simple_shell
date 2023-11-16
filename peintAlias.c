#include "shell.h"

/**
 * Prints the specified alias or all aliases.
 *
 * @param data The program's data structure.
 * @param aliasName The name of the alias to print, or NULL to print all aliases.
 * @return 0 if successful, or another number if an error occurs.
 */
int printAlias(dataOfProgram *data, char *aliasName) {
    int index, j, aliasLength;
    char buffer[250] = {0};

    if (data->aliasList) {
        aliasLength = strLength(aliasName);

        for (index = 0; data->aliasList[index]; ++index) {
            if (!aliasName ||
                (strCompare(data->aliasList[index], aliasName, aliasLength) &&
                 data->aliasList[index][aliasLength] == '=')) {
                for (j = 0; data->aliasList[index][j]; ++j) {
                    buffer[j] = data->aliasList[index][j];

                    if (data->aliasList[index][j] == '=') {
                        break;
                    }
                }

                buffer[j + 1] = '\0';
                bufferAdd(buffer, "'");
                bufferAdd(buffer, data->aliasList[index] + j + 1);
                bufferAdd(buffer, "'\n");
                _print(buffer);
            }
        }
    }

    return 0;
}

/**
 * Retrieves the value of an alias.
 *
 * @param data The program's data structure.
 * @param name The name of the alias to retrieve the value for.
 * @return A pointer to the alias's value, or NULL if the alias does not exist.
 */
char *getAlias(dataOfProgram *data, char *name) {
    int index, aliasLength;

    if (!name || !data->aliasList) {
        return NULL;
    }

    aliasLength = strLength(name);

    for (index = 0; data->aliasList[index]; ++index) {
        if (strCompare(name, data->aliasList[index], aliasLength) &&
            data->aliasList[index][aliasLength] == '=') {
            return data->aliasList[index] + aliasLength + 1;
        }
    }

    return NULL;
}

/**
 * Adds or modifies an alias.
 *
 * @param aliasString The alias definition in the form "aliasName='aliasValue'".
 * @param data The program's data structure.
 * @return 0 if successful, or another number if an error occurs.
 */
int setAlias(char *aliasString, dataOfProgram *data) {
    int index, j;
    char buffer[250] = {0};
    char *temp = NULL;

    if (!aliasString || !data->aliasList) {
        return 1;
    }

    for (index = 0; aliasString[index]; ++index) {
        if (aliasString[index] != '=') {
            buffer[index] = aliasString[index];
        } else {
            temp = getAlias(data, aliasString + index + 1);
            break;
        }
    }

    for (j = 0; data->aliasList[j]; ++j) {
        if (strCompare(buffer, data->aliasList[j], index) &&
            data->aliasList[j][index] == '=') {
            free(data->aliasList[j]);
            break;
        }
    }

    if (temp) {
        bufferAdd(buffer, "=");
        bufferAdd(buffer, temp);
        data->aliasList[j] = strDuplicate(buffer);
    } else {
        data->aliasList[j] = strDuplicate(aliasString);
    }

    return 0;
}

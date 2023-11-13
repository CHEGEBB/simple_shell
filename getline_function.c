#include "shell.h"

/**
 * buffer_command_input - buffers chained commands
 * @data: parameter structure
 * @command_buffer: address of command buffer
 * @buffer_len: address of buffer length
 *
 * Return: bytes read
 */
ssize_t buffer_command_input(data_t *data, char **command_buffer, size_t *buffer_len) {
    ssize_t bytes_read = 0;
    size_t len_position = 0;

    if (!*buffer_len) { /* Check and fill the buffer if empty */
        free(*command_buffer);
        *command_buffer = NULL;
        signal(SIGINT, handle_interrupt_signal);
#if USE_GETLINE
        bytes_read = getline(command_buffer, &len_position, stdin);
#else
        bytes_read = custom_getline(data, command_buffer, &len_position);
#endif
        if (bytes_read > 0) {
            if ((*command_buffer)[bytes_read - 1] == '\n') {
                (*command_buffer)[bytes_read - 1] = '\0'; /* Remove trailing newline */
                bytes_read--;
            }
            data->line_count_flag = 1;
            remove_comments(*command_buffer);
            build_history_list(data, *command_buffer, data->history_count++);
            /* Determine if this forms a command chain */
            {
                *buffer_len = bytes_read;
                data->cmd_buffer = command_buffer;
            }
        }
    }
    return (bytes_read);
}

/**
 * acquire_input - fetches a line without the newline character
 * @data: parameter structure
 *
 * Return: bytes read
 */
ssize_t acquire_input(data_t *data) {
    static char *buffer;
    static size_t position, idx, length;
    ssize_t bytes_read = 0;
    char **buffer_ptr = &(data->arguments), *ptr;

    _putchar(FLUSH_BUFFER);
    bytes_read = buffer_command_input(data, &buffer, &length);
    if (bytes_read == -1) { /* If it's the end of file */
        return (-1);
    }
    if (length) { /* Check if commands are remaining in the chain buffer */
        idx = position;
        ptr = buffer + position;

        check_cmd_chain(data, buffer, &idx, position, length);
        while (idx < length) { /* Iterate till semicolon or the end */
            if (is_cmd_chain(data, buffer, &idx)) {
                break;
            }
            idx++;
        }

        position = idx + 1;
        if (position >= length) { /* Reached the end of the buffer */
            position = length = 0; /* Reset position and length */
            data->cmd_buffer_type = CMD_NORMAL;
        }

        *buffer_ptr = ptr; /* Return pointer to current command position */
        return (_strlen(ptr)); /* Return length of current command */
    }

    *buffer_ptr = buffer; /* Not a chain, return buffer from _getline() */
    return (bytes_read); /* Return length of buffer from _getline() */
}

/**
 * read_buffer - reads a buffer
 * @data: parameter structure
 * @buf: buffer
 * @size: size of buffer
 *
 * Return: bytes read
 */
ssize_t read_buffer(data_t *data, char *buf, size_t *size) {
    ssize_t bytes_read = 0;

    if (*size) {
        return (0);
    }
    bytes_read = read(data->read_fd, buf, READ_BUF_SIZE);
    if (bytes_read >= 0) {
        *size = bytes_read;
    }
    return (bytes_read);
}

/**
 * custom_getline - fetches the next line of input from STDIN
 * @data: parameter structure
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated buffer if not NULL
 *
 * Return: size
 */
int custom_getline(data_t *data, char **ptr, size_t *length) {
    static char buf[READ_BUF_SIZE];
    static size_t idx, len;
    size_t k;
    ssize_t bytes_read = 0, size = 0;
    char *p = NULL, *new_ptr = NULL, *c;

    p = *ptr;
    if (p && length) {
        size = *length;
    }
    if (idx == len) {
        idx = len = 0;
    }

    bytes_read = read_buffer(data, buf, &len);
    if (bytes_read == -1 || (bytes_read == 0 && len == 0)) {
        return (-1);
    }

    c = _strchr(buf + idx, '\n');
    k = c ? 1 + (unsigned int)(c - buf) : len;
    new_ptr = _realloc(p, size, size ? size + k : k + 1);
    if (!new_ptr) { /* Memory allocation failure */
        return (p ? free(p), -1 : -1);
    }

    if (size) {
        _strncat(new_ptr, buf + idx, k - idx);
    } else {
        _strncpy(new_ptr, buf + idx, k - idx + 1);
    }

    size += k - idx;
    idx = k;
    p = new_ptr;

    if (length) {
        *length = size;
    }
    *ptr = p;
    return (size);
}

/**
 * handle_interrupt_signal - handles SIGINT signal
 * @sig_num: the signal number
 *
 * Return: void
 */
void handle_interrupt_signal(__attribute__((unused)) int sig_num) {
    _puts("\n");
    _puts("$ ");
    _putchar(FLUSH_BUFFER);
}

/* Handle Hlib protocol */

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "command.h"

static Command *command_from_string (char *str, int parse);

/* Returns an Command given a printf format string */
Command *command_format (const char *format, ...)
{
    va_list args;
    char buf[32];
    int bufsize = 32;
    Command *cmd = NULL;
    int ret = 0;

    if (format == NULL)
        return NULL;
    
    va_start (args, format);
    ret = vsnprintf(buf, bufsize - 2, format, args);
    va_end (args);
    
    if(ret <= 0)
        return NULL;
    
    buf[ret] = '\n';
    buf[ret + 1] = '\0';

    cmd = command_from_string (buf, 0);
    
    return cmd;
}

/* Parses a string and returns an Command */
/* This function is called many times so manual parsing is done
 * instead of using strtok */
static Command *command_from_string(char *str, int parse)
{
    Command *cmd = NULL;
    char *tmpstr = NULL;
    int param_count = 0;
    char *param_start;
    int length = 0;
    int i = 0;

    if(str == NULL)
        return NULL;
    
    length = strlen(str);

    if (length < 5)
        return NULL;

    cmd = (Command *) calloc(1, sizeof(Command));
    cmd->max_params = 12;
    cmd->param_count = 0;

    cmd->command = strdup(str);
    
    if (!parse)
        return cmd;
        
    tmpstr = strdup(str);

    /* Note that the parameters all point to bytes in tmpstr */
    param_start = tmpstr;

    /* Parse the command */
    for (i = 0; i < length; i++) {
        if (tmpstr[i] == ' ') {
            if (cmd->name == NULL) {
                cmd->name = param_start;
                tmpstr[i] = '\0';
                param_start = tmpstr + i + 1;
                cmd->params[param_count++] = param_start;
            }
            else {
                param_start = tmpstr + i + 1;
                cmd->params[param_count++] = param_start;
                tmpstr[i] = '\0';
            }
        }
        if (tmpstr[i] == '\n')
            tmpstr[i] = '\0';
    }

    cmd->param_count = param_count;
    cmd->pdata = tmpstr;

    return cmd;
}

void command_free (Command *cmd)
{
    if (cmd == NULL)
        return;

    if (cmd->pdata != NULL)
        free(cmd->pdata);

    if (cmd->command != NULL)
        free (cmd->command);

    free (cmd);

    return;
}

int command_send(Command *cmd, Socket *sock)
{
    int ret = 0;
    int len = 0;

    if (cmd == NULL || sock == NULL)
        return -1;

    len = strlen(cmd->command);
    
    /* Command has a \n appended to it */
    ret = socket_write(sock, cmd->command, len);

    if (ret == len)
        return 0;

    return -1;
}

Command *command_read(Socket *socket)
{
    Command *cmd = NULL;
    char *line = NULL;
    
    if (socket == NULL)
        return NULL;
    
    wait_until_data_available(socket);
    line = socket_readline(socket, "\n");
    
    cmd = command_from_string(line, 1);
    free(line);

    return cmd;
}

char *command_read_payload(Socket *socket, Command *cmd)
{
    char *buf = NULL;
    int chars_read = 0;
    int length = 0;
    int ret = 0;

    if (socket == NULL || cmd == NULL)
        return NULL;

    if (cmd->param_count < 2)
        return NULL;

    /* Read the second parameter - the length of the payload */
    length = atoi(cmd->params[1]);
    if (length <= 0) {
        return NULL;
    }

    buf = (char *) calloc(length + 1, 1);
    if (buf == NULL) {
        return NULL;
    }

    /* Wait for data */
    while (chars_read < length) {
        wait_until_data_available(socket);

        ret = socket_read(socket, buf + chars_read, length - chars_read);
        if (ret <= 0)
            break;

        chars_read += ret;
    }

    if (chars_read == length)
        return buf;

    free(buf);

    fprintf(stderr, "payload wrong size: chars_read = %d length %d\n", chars_read, length);
    return NULL;
}

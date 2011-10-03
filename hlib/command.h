#ifndef _COMMAND_H
#define _COMMAND_H

#include "socket.h"

typedef struct _Command
{
    char *name;       /* Name of the command */
    char *params[12]; /* Parameters */
    char *command;    /* The full command */
    unsigned int param_count; /* Number of parameters */
    unsigned int max_params;
    char *pdata; /* Payload data */
} Command;

Command *command_format(const char *format, ...);
void command_free(Command *cmd);

int command_send(Command *cmd, Socket *socket);

Command *command_read(Socket *socket);
char *command_read_payload(Socket *socket, Command *cmd);

#endif /* _COMMAND_H */


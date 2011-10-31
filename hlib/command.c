/*

The MIT License (MIT)
Copyright (c) 2011 Derek Ingrouville, Julien Lord, Muthucumaru Maheswaran

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
*/

/* Handle Hlib protocol */

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "command.h"

/* Maximum size of input buffer TODO: Analysis to figure out a good candidate value */
#define BUFSIZE 		128 

static Command *command_from_string (char *str, int parse);

/* Returns an Command given a printf format string */
/* This is now deprecated by *command_format_json */
Command *command_format (const char *format, ...)
{
    va_list args;
    char buf[64];
    int bufsize = 64;
    Command *cmd = NULL;
    int ret = 0;
	
    if (format == NULL)
        return NULL;
    
    va_start (args, format);
    ret = vsnprintf(buf, bufsize - 1, format, args);
    va_end (args);
    
    if(ret <= 0)
        return NULL;
    
    buf[ret] = '\n';
    buf[ret + 1] = '\0';
    
    cmd = command_from_string (buf, 0);
    
    return cmd;
}
/*
 * Return a command for Browser-side comsumption in JSON format.
 * 
 * The format is a 2-member object:
 * 		name: function name
 * 		args: array of arguments to the function.
 * 
 * TODO: 	Make a variation of this to accomodate larger buffer 
 * 			requirements of the TextArea functions. 
 * 			
 */
Command *command_format_json(const char *name,const char *format, ...)
{
	va_list args;
	int i;
	char buf[BUFSIZE];
	char *json;
	char json_format[] = "{\"name\":\"%s\", \"args\":[%s]}";
	int ret;
	Command *cmd = NULL;
	char* new_format;
	int count = 0; /* this counts additional overhead chars */
	
	if (format == NULL)
        return NULL;
        
    /* 
     * Strings NEED to be properly quoted, return NULL if we
     * find an unquoted string.
     * 
     * There's a SINGLE exception for the Register Callback function.
     * It was done for simplicity's sake. It might be revisited eventually
     * but for now, just deal with it. Please and thank you.
     * 
     */
    if (strcmp(name, "REG_CB") != 0) {
		for(i = 0; i < strlen(format); i++) {
			if (format[i] == '%' && format[i+1] == 's') {
				count += 2;
				if (format[i-1] != '"' || format[i+2] != '"') {
					return NULL;
				}
			}
		}
	}
    
    /* Format is const so make a usable copy of it to play with */
    new_format = strdup(format);
    
    /* Replace spaces by commas. This will help for json formatting */
	for(i = 0; i < strlen(new_format); i++) {
		if (new_format[i] == ' ') {
			new_format[i] = ',';
			count++;
		}
	}
	
    
    va_start (args, format);
    ret = vsnprintf(buf, BUFSIZE, new_format, args);
    va_end (args);
	
	json = malloc(sizeof(char) * (strlen(json_format) + strlen(name) + BUFSIZE));
	ret = sprintf(json, json_format, name, buf);
	
	if(json <= 0)
        return NULL;
    
    json[ret] = '\n';
    json[ret + 1] = '\0';
    
	cmd = (Command *) calloc(1, sizeof(Command));
    cmd->max_params = 12;
    cmd->param_count = 0;

    cmd->command = strdup(json);
	
	free(json);
	
    return cmd;
}

/* Parses a string and returns a Command */
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
    /* TODO: Revisit this. Files can have spaces in the name. Maybe changed the separator to something else... */
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

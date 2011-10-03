#include "hlib.h"
#include "command.h"
#include "socket.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char *int_to_string(int x)
{
    char tmp_buf[33];
    int ret = -1;

    ret = snprintf(tmp_buf, 32, "%d", x);
    if (ret <= 0)
        return NULL;

    tmp_buf[32] = '\0';
    return strdup(tmp_buf);
}

Display *OpenDisplay(char *hostname, int port)
{
    Display *display = NULL;
    Socket *socket = NULL;
    char *port_buf = NULL;

    port_buf = int_to_string(port);
    if (port_buf == NULL)
        return NULL;

    socket = socket_new(Socket_Blocking);
    if (socket_connect(socket, hostname, port_buf) != 0) {
        socket_free(socket);
        free(port_buf);
        return NULL;
    }

    free(port_buf);

    display = (Display *) malloc(sizeof(Display));
    
    display->socket = socket;
    display->hostname = strdup(hostname);
    display->port = port;
    display->callbacks = callbacks_new();
    
    return display;
}

void CloseDisplay(Display *display)
{
    if (display == NULL)
        return;

    printf ("CLOSING CONNECTION FOR DISPLAY %s:%d\n", 
            display->hostname, 
            display->port);

    if (display->hostname != NULL)
        free(display->hostname);

    if (display->socket != NULL)
        socket_free(display->socket);
        
    if (display->callbacks != NULL)
        callbacks_free(display->callbacks);

    free(display);
}

/* Draw a rectangle at (x,y) of width X height */
int DrawRectangle(Display *display, int x, int y, int width, int height)
{
    Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
    
    cmd = command_format("RECT %d %d %d %d", x, y, width, height);
    if (cmd == NULL)
        return -1;
        
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}

/* TODO: remove this */
int SendText(Display *display, int x, int y, char *text)
{
    Command *cmd = NULL;
    Socket *socket = NULL;
    int textlen = 0;
    
    socket = display->socket;
        
    if (text == NULL)
        return -1;
    
    textlen = strlen(text);
    if (textlen == 0)
        return 0;
        
    cmd = command_format("TXT %d %d %s", x, y, text);
    if (cmd == NULL)
        return -1;
    
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}

int ClearScreen(Display *display)
{
    Command *cmd = NULL;
    Socket *socket = NULL;
    
    socket = display->socket;
               
    cmd = command_format("CLEAR");
    if (cmd == NULL)
        return -1;
    
    if (command_send(cmd, socket) != 0) {
        command_free(cmd);
        return -1;
    }
    
    return 0;
}

/* TODO: Clean up */
Event *GetEvent(Display *display)
{
    Command *cmd = command_read(display->socket);
    Event *e = NULL;
    int len = 0;
    
    if (cmd == NULL)
        return NULL;
    
    if (cmd->params[0] == NULL)
        return NULL;
    
    len = strlen(cmd->params[0]);

    /* Check for expose */
    if (len == 6 && strcmp(cmd->params[0], "EXPOSE") == 0) {
        e = event_expose_new();
        return e;
    }
    else if (len == 5 && strcmp(cmd->params[0], "CLICK") == 0) {
        int x = atoi(cmd->params[1]);
        int y = atoi(cmd->params[2]);
        e = event_click_new(x, y);
        return e;
    }
    
    return NULL;
}

void RegisterCallback(Display *display, EventType etype, EventCallback cb, void *data)
{
    callbacks_add(display->callbacks, etype, cb, data);
}

void MainLoop(Display *display)
{
    Event *e = NULL;
    while ((e = GetEvent(display))) {
        callbacks_call(display->callbacks, display, e);   
    } 
}

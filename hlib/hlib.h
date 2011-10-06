#ifndef __HLIB_H
#define __HLIB_H

#include "socket.h"
#include "event.h"
#include "callback.h"

typedef struct Display
{
    char *hostname;
    int port;
    int width;
    int height;
    Socket *socket;
    Callbacks *callbacks;
} Display;

Display *OpenDisplay(char *hostname, int port);
void CloseDisplay(Display *display);


/* Draw a rectangle at (x,y) of width X height */
int DrawRectangle(Display *display, int x, int y, int width, int height);

/* TODO: this function exists only for testing - remove */ 
int SendText(Display *display, int x, int y, char *text);

int ClearScreen(Display *display);

/* Get an event from remote server */
Event *GetEvent(Display *display);

void RegisterCallback(Display *display, EventType etype, EventCallback cb, void *data);

int ProcessEvent(Display *d, Event *e);

void MainLoop(Display *display);
#endif

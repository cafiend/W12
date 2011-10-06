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


/* 2D draw primitives */
int DrawArc(Display *display, int x, int y, int width, int height, float start, float stop);
int DrawEllipse(Display *display, int x, int y, int width, int height);
int DrawLine2D(Display *display, int x0, int y0, int x1, int y1);
int DrawPoint2D(Display *display, int x, int y);
int DrawQuad(Display *display, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);
int DrawRectangle(Display *display, int x, int y, int width, int height);
int DrawTriangle(Display *display, int x0, int y0, int x1, int y1, int x2, int y2);

/* TODO: this function exists only for testing - remove */ 
int SendText(Display *display, int x, int y, char *text);

int ClearScreen(Display *display);

/* Get an event from remote server */
Event *GetEvent(Display *display);

void RegisterCallback(Display *display, EventType etype, EventCallback cb, void *data);

int ProcessEvent(Display *d, Event *e);

void MainLoop(Display *display);
#endif

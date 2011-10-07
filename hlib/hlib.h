#ifndef __HLIB_H
#define __HLIB_H

#include "socket.h"
#include "event.h"
#include "callback.h"

/* Some Processing constants */
#define RGB 	1
#define HSB 	3
#define CENTER 	3
#define RADIUS 	2
#define CORNER 	0
#define CORNERS 1

#define POINTS			2
#define LINES			4
#define TRIANGLES		9
#define TRIANGLE_FAN	11
#define TRIANGLE_STRIP	10
#define QUADS			16
#define QUAD_STRIP		17
#define CLOSE			2

#define SQUARE 	"butt"
#define PROJECT "square"
#define ROUND 	"round"
#define MITER	"miter"
#define BEVEL	"bevel"

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

/* Colour functions */
/* TODO: Add more validation for integer vs. floating-point methods */
int Background1i(Display *display, int grey);
int Background3i(Display *display, int r, int g, int b);
int ColorMode1f(Display *display, int mode, float r0);
int ColorMode1i(Display *display, int mode, int r0);
int ColorMode3f(Display *display, int mode, float r0, float r1, float r2);
int ColorMode3i(Display *display, int mode, int r0, int r1, int r2);
int Stroke1i(Display *display, int val);
int Stroke1f(Display *display, float val);
int Stroke3i(Display *display, int r, int g, int b);
int Stroke3f(Display *display, float r, float g, float b);
int StrokeWeight(Display *display, int w);
int NoStroke(Display *display);
int NoFill(Display *display);
int Fill1i(Display *display, int val);
int Fill3i(Display *display, int r, int g, int b);
int Fill1f(Display *display, float val);
int Fill3f(Display *display, int r, int g, int b);

/* Helper functions */
int PushStyle(Display *display);
int PopStyle(Display *display);
int Size(Display *display, int width, int height);
int EllipseMode(Display *display, int mode);
int RectMode(Display *display, int mode);
int StrokeCap(Display *display, const char* mode);
int StrokeJoin(Display *display, const char* mode);

/* Vertex related */
int BeginShape(Display *display);
int BeginShapeMode(Display *display, int mode);
int EndShape(Display *display);
int EndShapeMode(Display *display, int mode);
int Vertex2D(Display *display, int x, int y);

/* TODO: this function exists only for testing - remove */ 
int SendText(Display *display, int x, int y, char *text);

int ClearScreen(Display *display);

/* Get an event from remote server */
Event *GetEvent(Display *display);

void RegisterCallback(Display *display, EventType etype, EventCallback cb, void *data);

int ProcessEvent(Display *d, Event *e);

void MainLoop(Display *display);
#endif

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

#ifndef __HLIB_H
#define __HLIB_H

#include "socket.h"
#include "event.h"
#include "callback.h"

#define CEIL(X) ( (X - (int)X)>0 ? (int)X+1 : (int)X )

/* Some Processing constants (NB A lot of these serve multiple purposes) */
#define LEFT	37 /* mouse buttons */
#define MIDDLE	3
#define RIGHT	39

#define RGB 	1 /* colour modes */
#define HSB 	3

#define CENTER 	3 /* draw modes */
#define RADIUS 	2
#define CORNER 	0
#define CORNERS 1

#define TOP			101
#define BOTTOM		102
#define BASELINE	0

#define POINTS			2 /* vertex shape modes */
#define LINES			4
#define TRIANGLES		9
#define TRIANGLE_FAN	11
#define TRIANGLE_STRIP	10
#define QUADS			16
#define QUAD_STRIP		17
#define CLOSE			2

#define SQUARE 	"butt"	/* stroke modes */
#define PROJECT "square"
#define ROUND 	"round"
#define MITER	"miter"
#define BEVEL	"bevel"

#define TRUE	1
#define FALSE	0

#define CHUNKSIZE	1048576 /* 1Meg chunk size */

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

int PushMatrix(Display *display);
int PopMatrix(Display *display);
int Translate2i(Display *display, int x, int y);
int Translate2f(Display *display, float x, float y);
int Rotate(Display *display, float angle);
int Scale1f(Display *display, float scale);
int Scale2f(Display *display, float x, float y);


/* Font management */
/* Careful with the fontName attributes. Jquery is sensitive about what goes in there.
 * Periods (.) and slashes (/) are known to not work. perhaps others as well. Stick
 * to alphanumeric identifiers for fontName.
 */
int CreateFont(Display *display, const char *fontName, const char *fontURL);
int LoadFont(Display *display, const char *fontName, int size);
int TextFont(Display *display, const char *fontName, int size);
int TextAlign(Display *display, int h_align, int v_align);
int TextLeading(Display *display, int dist);
int TextSize(Display *display, int size);
/* need to decide if Hlib will handle textWidth(), textAscent(), and textDescent() */

/* TODO: this function exists only for testing - remove */ 
int SendText(Display *display, int x, int y, char *text);
int OverwriteTextArea(Display *display, const char *target, const char *text);
int AppendTextArea(Display *display, const char *target, const char *text);
int CreateTextArea(Display *display, const char *id, int x, int y, int width, int height, int readonly);
int TextAreaSetFont(Display *display, const char *id, const char *fontName);
int TextAreaSetCss(Display *display, const char *id, const char *name, const char *value);

int ClearScreen(Display *display);

int RegisterRemoteInt(Display *display, const char* name, int value);
int RegisterRemoteFloat(Display *display, const char* name, float value);

/* Get an event from remote server */
Event *GetEvent(Display *display);
/* Make the browser-side aware of which events need to be communicated back to the application */ 
int SendRegisterCallbackMsg(Display *display, char* events);
/* Register a callback method locally */
void RegisterCallback(Display *display, EventType etype, EventCallback cb, void *data);

int ProcessEvent(Display *d, Event *e);

void MainLoop(Display *display);
#endif

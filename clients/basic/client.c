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

#include <hlib/hlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int rect_x = 50;
int rect_y = 50;
int width = 45;
int height = 45;

int arc_x = 150;
int arc_y = 250;
int arc_w = 60;
int arc_h = 75;
float arc_start = 1.57;
float arc_stop = 3.14;

int text_x = 10;
int text_y = 10;


int is_inside_rectangle(int x, int y)
{
    int right = rect_x + width;
    int bottom = rect_y + height;
    
    if (x >= rect_x && x <= right) {
        if (y >= rect_y && y <= bottom)
            return 1;
    }
    
    return 0;
}

void resize(Display *display, Event *event, void *data) {
	printf("Resize handler!\n");
	printf("Width: %d\t Height: %d\n", event->val.win.width, event->val.win.height);
}

/** Event handlers **/
void setup(Display *display, Event *event, void *data) {
	printf("In setup\n");
	printf("Width: %d\t Height: %d\n", event->val.win.width, event->val.win.height);
	/* Move this to a separate setup() function, callback-based */
    Background1i(display, 255);
    Size(display, 500, 500);
    Fill1i(display, 0);
    LoadFont(display, "Courier New", 10);
}

void mouse_move(Display *display, Event *event, void *data) {
	PushStyle(display);
	StrokeWeight(display, 10);
	Stroke3i(display, 204, 102, 0);
	int curX = event->val.mouse.x;
	int curY = event->val.mouse.y;
	int prevX = event->val.mouse.x - event->val.mouse.dx;
	int prevY = event->val.mouse.y - event->val.mouse.dy;
	DrawLine2D(display, prevX, prevY, curX, curY);
	PopStyle(display);
}

void mouse_drag(Display *display, Event *event, void *data) {
	PushStyle(display);
	StrokeWeight(display, 5);
	if (event->val.mouse.button == LEFT) {
		Stroke3i(display, 102, 204, 0);
	} else if (event->val.mouse.button == MIDDLE) {
		Stroke3i(display, 102, 0, 204);
	} else if (event->val.mouse.button == RIGHT) {
		Stroke3i(display, 204, 0, 102);
	}
	int curX = event->val.mouse.x;
	int curY = event->val.mouse.y;
	int prevX = event->val.mouse.x - event->val.mouse.dx;
	int prevY = event->val.mouse.y - event->val.mouse.dy;
	DrawLine2D(display, prevX, prevY, curX, curY);
	PopStyle(display);
}

void mouse_drag_out(Display *display, Event *event, void *data)
{
	printf("Drag out!\n");
	SendText(display, text_x, text_y, "Drag Out!");
}
void mouse_down(Display *display, Event *event, void *data) {
	//printf("Mouse down.\n");
}
void expose_event(Display *display, Event *event, void *data)
{
	
	SendText(display, text_x, text_y, "Connected nice and proper");
	/*
	DrawEllipse(display, 0, 50, 33, 33);
	PushStyle(display);
	StrokeWeight(display, 10);
	Fill3i(display, 204, 153, 0);
	DrawEllipse(display, 50, 50, 33, 33);
	PopStyle(display);
	DrawEllipse(display, 100, 50, 33, 33);
	
	DrawEllipse(display, 0, 250, 33, 33);
	PushStyle(display);
	StrokeWeight(display, 10);
	Fill3i(display, 204, 153, 0);
	DrawEllipse(display, 33, 250, 33, 33);
	PushStyle(display);
	Stroke3i(display, 0, 102, 153);
	DrawEllipse(display, 66, 250, 33, 33);
	PopStyle(display);
	DrawEllipse(display, 99, 250, 33, 33);
	PopStyle(display);
	DrawEllipse(display, 120, 250, 33,33);
	
	EllipseMode(display, CENTER);
	DrawEllipse(display, 300, 300, 50, 50);
	EllipseMode(display, CORNER);
	Fill1i(display, 102);
	DrawEllipse(display, 300, 300, 50, 50);
	
	Fill1i(display, 255);
	RectMode(display, CENTER);
	DrawRectangle(display, 400, 400, 50, 50);
	RectMode(display, CORNER);
	Fill1i(display, 102);
	DrawRectangle(display, 400, 400, 50, 50);
	
	Fill1i(display, 0);
	StrokeWeight(display, 12);
	StrokeCap(display, ROUND);
	DrawLine2D(display, 250, 50, 350, 50);
	StrokeCap(display, SQUARE);
	DrawLine2D(display, 250, 70, 350, 70);
	StrokeCap(display, PROJECT);
	DrawLine2D(display, 250, 90, 350, 90);
	
	NoFill(display);
	StrokeWeight(display, 10);
	StrokeJoin(display, MITER);
	BeginShape(display);
	Vertex2D(display, 35, 320);
	Vertex2D(display, 65, 350);
	Vertex2D(display, 35, 380);
	EndShape(display);
	
	NoFill(display);
	StrokeWeight(display, 10);
	StrokeJoin(display, BEVEL);
	BeginShape(display);
	Vertex2D(display, 75, 320);
	Vertex2D(display, 105, 350);
	Vertex2D(display, 75, 380);
	EndShape(display);
	
	NoFill(display);
	StrokeWeight(display, 10);
	StrokeJoin(display, ROUND);
	BeginShape(display);
	Vertex2D(display, 115, 320);
	Vertex2D(display, 145, 350);
	Vertex2D(display, 115, 380);
	EndShape(display);
	
	
	int i,j;
	DrawEllipse(display, 250, 300, 80, 90);
    DrawRectangle(display, rect_x, rect_y, width, height);
    DrawArc(display, arc_x, arc_y, arc_w, arc_h, arc_start, arc_stop);
    DrawLine2D(display, 50, 450, 450, 50);
    DrawPoint2D(display, 250, 50);
    DrawQuad(display, 250, 400, 275, 410, 225, 440, 200, 410);
    DrawTriangle(display, 370, 370, 420, 290, 490, 410);
    
    Fill3i(display, 204, 102, 0);
	DrawRectangle(display, 80, 390, 110, 95);
	
    NoStroke(display);
    ColorMode1i(display, RGB, 100);
    for (i=0; i<100;i++) {
		for (j=0; j<100; j++) {
			Stroke3i(display, i, j, 0);
			DrawPoint2D(display, i+300, j);
		}
	}
	NoStroke(display);
    ColorMode1i(display, HSB, 100);
    for (i=0; i<100;i++) {
		for (j=0; j<100; j++) {
			Stroke3i(display, i,j,100);
			DrawPoint2D(display, i+300,j+200);
		}
	}
	*/ 	
}

void click_event(Display *display, Event *event, void *data)
{
	/*
    if (is_inside_rectangle(event->val.mouse.x, event->val.mouse.y)) {
        if (SendText(display, text_x, text_y, "INSIDE") != 0) {
            fprintf(stderr, "Unable to send data\n");
        }
    }
    else {
        if (SendText(display, text_x, text_y, "OUTSIDE") != 0) {
            fprintf(stderr, "Unable to send data\n");
        }
    }
    */
    /* Dinky way to keep text visible and clear when the window's full */
    text_y += 20;
    if (text_y > 500) {
      text_x += 70;
      if (text_x > 500) {
        text_x %= 500;
        /* repaint the display */
        ClearScreen(display);
      }
      text_y %= 500;
    }
}

int main()
{
    Display *display = NULL;
    char *host = "localhost";
    int port = 9090;
    
    display = OpenDisplay("localhost", 9090);
    if (display == NULL) {
        fprintf(stderr, "Unable to connect to display %s:%d\n", host, port);
        exit(1);
    }

    /* Register Callbacks */
    RegisterCallback(display, ExposeEventType, expose_event, NULL);
    RegisterCallback(display, ClickEventType, click_event, NULL);
    RegisterCallback(display, SetupEventType, setup, NULL);
    //RegisterCallback(display, MouseDownEventType, mouse_down, NULL);
    RegisterCallback(display, MouseDragEventType, mouse_drag, NULL);
    RegisterCallback(display, MouseDragOutEventType, mouse_drag_out, NULL);
    //RegisterCallback(display, MouseMoveEventType, mouse_move, NULL);
    RegisterCallback(display, Resize, resize, NULL);
    
    MainLoop(display);
    
    CloseDisplay(display);
    return 0;
}

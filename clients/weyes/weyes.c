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
#include <math.h>

#define STROKE_W	10.0
#define EYE_W		200.0	
#define EYE_H		250.0
#define PUPIL_W		10.0
#define EYE_RAD		(EYE_W / 4.0 - PUPIL_W)

/* PROTOTYPES */
int draw(Display *display, int num, int mouseX, int mouseY) ;


int size_x 		= 2*EYE_W + 4*STROKE_W;
int size_y 		= EYE_H + 2*STROKE_W;
float *eyes 	= NULL; /* Eye centre positions */
float *pupils 	= NULL; /* initial pupil positions */

    
/** Event handlers **/
void setup(Display *display, Event *event, void *data) {
    Background1i(display, 255); /* white bg */
    Size(display, size_x, size_y);
    
    /* black stroke and white fill */
    StrokeWeight(display, STROKE_W);
    Stroke1i(display, 0);
    Fill1i(display, 255); 
    
    if (eyes == NULL) {
		eyes = malloc(4*sizeof(float));
		eyes[0] = 0.25 * size_x;
		eyes[1] = 0.50 * size_y;
		eyes[2] = 0.75 * size_x;
		eyes[3] = 0.50 * size_y;
	}
	
	if (pupils == NULL) {
		pupils = malloc(4*sizeof(float));
		memcpy(pupils, eyes, 4*sizeof(float));
	}
}

void mouse_move(Display *display, Event *event, void *data) {
	//ClearScreen(display);
	int x = event->val.mouse.x;
	int y = event->val.mouse.y;
	
	if (x < 0) x = 0;
	if (y < 0) y = 0; 
	draw(display, 0, x, y);
	draw(display, 2, x, y);
}

void expose_event(Display *display, Event *event, void *data)
{
    draw(display, 0, eyes[0], eyes[1]);
    draw(display, 2, eyes[2], eyes[3]);
}

int draw(Display *display,int num, int mouseX, int mouseY) {
	double angle1, angle2;
	double dx,dy;
	double tx,ty;
	double cosa, sina;
	double x,y,x2,y2;
	
	x = pupils[num];
    y = pupils[num+1];
    x2 = eyes[num];
    y2 = eyes[num+1];
    
    DrawEllipse(display, (int)eyes[num], (int)eyes[num+1], EYE_W, EYE_H);
    	  
    PushStyle(display);
    Fill1i(display, 0);
    
	dx = (double)mouseX - x;
	dy = (double)mouseY - y;
	angle1 = atan2(dy,dx);

	cosa = cos(angle1);
	sina = sin(angle1);
	tx = mouseX - cosa * EYE_RAD;
	ty = mouseY - sina * EYE_RAD;

	dx = tx - x2;
	dy = ty - y2;
	angle2 = atan2(dy,dx);

	cosa = cos(angle2);
	sina = sin(angle2);
	x 	= x2 + cosa * EYE_RAD;
	y 	= y2 + sina * EYE_RAD;

	PushMatrix(display);
	Translate2f(display,x, y);
	Rotate(display, angle1);
	DrawEllipse(display, EYE_RAD, 0, PUPIL_W, PUPIL_W);
	PopMatrix(display);
	
	pupils[num] = x;
	pupils[num+1] = y;
    
    PopStyle(display);
    return 0;
}

int main(int argc, char *argv[])
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
    RegisterCallback(display, SetupEventType, setup, NULL);
    RegisterCallback(display, MouseMoveEventType, mouse_move, NULL);
    
    MainLoop(display);
    
    CloseDisplay(display);
    free(eyes);
    free(pupils);
    return 0;
}

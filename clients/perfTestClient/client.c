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
#include <time.h>

int MAX_SHAPES[] = {1000, 5000, 10000, 15000, 20000};
int test_case = 10;
int w_width = 1000;
int w_height = 1000;
int x_pos = 0;
int y_pos = 0;
int width = 100;
int height = 50;
struct timespec time_0;
struct timespec time_1;

struct timespec diff(struct timespec start, struct timespec end) {
	struct timespec temp;
	if ((end.tv_nsec - start.tv_nsec) < 0) {
		temp.tv_sec = end.tv_sec - start.tv_sec - 1;
		temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec - start.tv_sec;
		temp.tv_nsec = end.tv_nsec - start.tv_nsec;
	}
	return temp;
}

/** Event handlers **/
void setup(Display *display, Event *event, void *data) {
	/* Move this to a separate setup() function, callback-based */
	Size(display, w_width, w_height);
    Background1i(display, 255);
    Stroke1i(display, 0);
    Fill3i(display, 225, 25, 0);
}

void expose_event(Display *display, Event *event, void *data)
{
	int i, j, k, max;
	for (i = 0; i < 5; i++) {
		max = MAX_SHAPES[i];
		printf("Drawing %d\n", max);
		for (j = 0; j < test_case; j++) {
			Background1i(display, 255);
			clock_gettime(CLOCK_REALTIME, &time_0);
			for(k=0; k < max; k++) {
				DrawRectangle(display, x_pos, y_pos, width, height);
				x_pos = (x_pos + width + 10);
				if (x_pos > w_width) {
					x_pos %= w_width;
					y_pos = (y_pos + height + 10) % w_height;
				}
			}
			clock_gettime(CLOCK_REALTIME, &time_1);
			printf("Test case %d\t%ld.%08ld\n", j, diff(time_0, time_1).tv_sec, diff(time_0, time_1).tv_nsec);
		}
	}
	Background1i(display, 255);
	SendText(display, 100, 100, "Test Done");
		
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
    RegisterCallback(display, SetupEventType, setup, NULL);
    
    MainLoop(display);
    
    CloseDisplay(display);
    return 0;
}

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


/** Event handlers **/
void setup(Display *display, Event *event, void *data) {
    int size_x = 500;
    int size_y = 500;
    if (data != NULL) {
		char* p = (char*)data;
    	size_x = (int)(p[0]);
    	size_y = (int)(p[1]);
    }
    Background1i(display, 255); /* white bg */
    Size(display, size_x, size_y);
    Fill1i(display, 0); /* black fg */
    LoadFont(display, "Courier New", 10);
}

void mouse_move(Display *display, Event *event, void *data) {

}


void expose_event(Display *display, Event *event, void *data)
{
    DrawEllipse(display, 125, 125, 200, 250);
    DrawEllipse(display, 375, 125, 200, 250);
}

void click_event(Display *display, Event *event, void *data)
{
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
    //RegisterCallback(display, SetupEventType, setup, *argv);
    RegisterCallback(display, MouseMoveEventType, mouse_move, NULL);
    
    MainLoop(display);
    
    CloseDisplay(display);
    return 0;
}

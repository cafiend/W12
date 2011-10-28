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


/** Event handlers **/
void setup(Display *display, Event *event, void *data) {
    Background1i(display, 255);
    Size(display, 500, 500);
    Fill1i(display, 0);
}

void expose_event(Display *display, Event *event, void *data)
{
	char *text = "File transfer Test Client.";
	SendText(display, 10, 10, text);
}

void file_init(Display *display, Event *event, void *data) 
{
	char *text = "Hit the INIT handler code.";
	SendText(display, 10, 50, text);
	printf("Init callback hit\n");
}
void file_chunk(Display *display, Event *event, void *data) 
{
	char *text = "Hit the CHUNK handler code.";
	SendText(display, 10, 150, text);
	printf("Chunk callback hit\n");
}
void file_end(Display *display, Event *event, void *data) 
{
	char *text = "Hit the END handler code.";
	SendText(display, 10, 250, text);
	printf("End callback hit\n");
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
    RegisterCallback(display, SetupEventType, setup, NULL);
    RegisterCallback(display, ExposeEventType, expose_event, NULL);
    RegisterCallback(display, FileDropInit, file_init, NULL);
    RegisterCallback(display, FileDropChunkReceived, file_chunk, NULL);
    RegisterCallback(display, FileDropEnd, file_end, NULL);	
    
    MainLoop(display);
    
    CloseDisplay(display);
    return 0;
}

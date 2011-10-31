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


char *buf;
unsigned int cur;
char *c;
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
	if (event->type == b64FileDropInit) {
		printf("Base64 Encoded INIT received\n");
	} else {
		printf("Init callback hit\n");
		printf("\tname %s \n", event->val.drop.name );
		printf("\ttype %s \n", event->val.drop.type );
		printf("\tsize %d \n", event->val.drop.size );
		printf("\tnum_chunk %d \n", event->val.drop.num_chunks );
		printf("\tcur_chunk %d \n", event->val.drop.cur_chunk );
		buf = malloc(event->val.drop.size + 1);
		cur = 0;
		c = buf;
	}
}
void file_chunk(Display *display, Event *event, void *data) 
{
	char *text = "Hit the CHUNK handler code.";
	SendText(display, 10, 150, text);
	if (event->type == b64FileDropChunkReceived) {
		printf("Base64 Encoded chunk received\n");
	} else {
		printf("Chunk callback hit\n");
		printf("\tname %s \n", event->val.drop.name );
		printf("\ttype %s \n", event->val.drop.type );
		printf("\tsize %d \n", event->val.drop.size );
		printf("\tnum_chunk %d \n", event->val.drop.num_chunks );
		printf("\tchunk_size %d \n", event->val.drop.chunk_size );
		printf("\tcur_chunk %d \n", event->val.drop.cur_chunk );
		unsigned int i = 0;
		for(i = 0; i < event->val.drop.chunk_size; i++) {
			buf[cur] = event->val.drop.chunk[i];
			cur++;
		}
	}
}
void file_end(Display *display, Event *event, void *data) 
{
	char *text = "Hit the END handler code.";
	SendText(display, 10, 250, text);
	if (event->type == b64FileDropEnd) {
		printf("Base64 Encoded END received\n");
	} else {
		printf("End callback hit\n");
		printf("\tname %s \n", event->val.drop.name );
		printf("\ttype %s \n", event->val.drop.type );
		printf("\tsize %d \n", event->val.drop.size );
		printf("\tnum_chunk %d \n", event->val.drop.num_chunks );
		printf("\tcur_chunk %d \n", event->val.drop.cur_chunk );
		
		buf[event->val.drop.size] = '\0';
		printf("Starting:\n\n");
		printf("%s\n", buf);
		printf("Done.\n\n");
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
    RegisterCallback(display, SetupEventType, setup, NULL);
    RegisterCallback(display, ExposeEventType, expose_event, NULL);
    RegisterCallback(display, FileDropInit, file_init, NULL);
    RegisterCallback(display, FileDropChunkReceived, file_chunk, NULL);
    RegisterCallback(display, FileDropEnd, file_end, NULL);	
    RegisterCallback(display, b64FileDropInit, file_init, NULL);
    RegisterCallback(display, b64FileDropChunkReceived, file_chunk, NULL);
    RegisterCallback(display, b64FileDropEnd, file_end, NULL);	
    
    MainLoop(display);
    free(buf);
    CloseDisplay(display);
    return 0;
}

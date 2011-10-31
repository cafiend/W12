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

int text_x = 10;
int text_y = 10;

/** Event handlers **/
void setup(Display *display, Event *event, void *data) {
	/* Move this to a separate setup() function, callback-based */
    Background1i(display, 255);
    Size(display, 500, 500);
    Fill1i(display, 0);
    LoadFont(display, "Bitstream Vera Serif", 12);
    LoadFont(display, "LinBiolinum", 12);
    CreateTextArea(display, "a", 50, 150, 400, 300, TRUE);
    OverwriteTextArea(display, "a", "First test text.\\nSecond Line.");
    AppendTextArea(display, "a", "\\nAppend a third line.\\nFourth line.");
}

void load_fonts(Display *display, Event *event, void *data) {
	CreateFont(display, "name1", "/fonts/LinLibertineTTF_5.1.3_2011_06_21/LinBiolinum_aWB.ttf");	
	CreateFont(display, "name2", "/fonts/LinLibertineTTF_5.1.3_2011_06_21/LinBiolinum_K.ttf");
}

void expose_event(Display *display, Event *event, void *data)
{
	char *text = "Lorem ipsum dolor sit amet, consectetur adipiscing eLorem ipsum dolor sit amet, Lorem ipsum dolor sit amet, consectetur a";
	TextFont(display, "/fonts/LinLibertineTTF_5.1.3_2011_06_21/LinBiolinum_aWB.ttf", 12);
	SendText(display, text_x, text_y, "Connected nice and proper. 121 Test. LinBiolinum_aWB.ttf");
	TextFont(display, "/fonts/LinLibertineTTF_5.1.3_2011_06_21/LinBiolinum_K.ttf", 12);
	SendText(display, text_x, text_y+30, "Connected nice and proper. 121 Test. LinBiolinum_K.ttf");
	OverwriteTextArea(display, "a",  text);
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
    RegisterCallback(display, PreLoad, load_fonts, NULL);
    
    MainLoop(display);
    
    CloseDisplay(display);
    return 0;
}

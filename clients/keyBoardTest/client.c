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
char buf[50];
int cur = 0;
char *vowels;
char *qwerty;

void setup(Display *display, Event *event, void *data) {
	/* Move this to a separate setup() function, callback-based */
    Background1i(display, 255);
    Size(display, 500, 500);
    Fill1i(display, 0);
    CreateTextArea(display, "typed", 0, 100, 500, 50, TRUE);
    CreateTextArea(display, "presd", 0, 200, 500, 50, TRUE);
    CreateTextArea(display, "relsd", 0, 300, 500, 50, FALSE);
    
    
	DisableKeyList(display, KeyTyped, vowels);
}


void expose_event(Display *display, Event *event, void *data)
{
	SendText(display, 10, 10, "Connected nice and proper. Keyboard test.");
	OverwriteTextArea(display, "typed", "Typed:");
	OverwriteTextArea(display, "presd", "Pressed:");
	OverwriteTextArea(display, "relsd", "Released:");

}

void typed (Display *d, Event *e, void *da) {
	char c = (char) e->val.keyboard.keycode;
	buf[cur] = c;
	cur = (cur+1) % 50;
	if (cur == 0) {
		printf("ding\n");
		printf("%d, %c, %s\n", cur, c, buf);
		int ret = OverwriteTextArea(d, "typed", buf);
		printf("Ret:: %d, %s\n", ret, buf);
	}
	
}

void presd (Display *d, Event *e, void *da) {
	//OverwriteTextArea(d, "presd", "Pressed Enter");
	if (e->val.keyboard.keycode == KEY_CTRL) {
		DisableKeyList(d, KeyTyped, NULL);
	} else if (e->val.keyboard.keycode == KEY_ALT) {
		DisableKeyList(d, KeyTyped, qwerty);
	}
}

void relsd (Display *d, Event *e, void *da) {
	//OverwriteTextArea(d, "relsd", "Released it.");
	if (e->val.keyboard.keycode == KEY_CTRL) {
		DisableKeyList(d, KeyTyped, vowels);
	} else if (e->val.keyboard.keycode == KEY_ALT) {
		DisableKeyList(d, KeyTyped, NULL);
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
    char *t = NULL; //keyboardCallbackListBuilder(5, 'a', 'A', 'b', 'r', 'o'); 
    char *p = keyboardListBuilder(2, KEY_CTRL, KEY_ALT);
    vowels = keyboardListBuilder(5, 'a', 'e', 'i', 'o', 'u');
    qwerty = keyboardListBuilder(6, 'q', 'w', 'e', 'r', 't', 'y');
    
    /* Register Callbacks */
    RegisterCallback(display, ExposeEventType, expose_event, NULL);
    RegisterCallback(display, SetupEventType, setup, NULL);
    RegisterCallback(display, KeyTyped, typed, t);
    RegisterCallback(display, KeyPressed, presd, p);
    RegisterCallback(display, KeyReleased, relsd, p);
    
    MainLoop(display);
    
    CloseDisplay(display);
    
    free(t);
    free(p);
    free(vowels);
    free(qwerty);
    return 0;
}

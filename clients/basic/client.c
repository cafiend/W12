#include <hlib/hlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int rect_x = 50;
int rect_y = 50;
int width = 45;
int height = 45;

int text_x = 100;
int text_y = 100;


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

/** Event handlers **/
void expose_event(Display *display, Event *event, void *data)
{
    DrawRectangle(display, rect_x, rect_y, width, height);
}

void click_event(Display *display, Event *event, void *data)
{
    if (is_inside_rectangle(event->val.click.x, event->val.click.y)) {
        if (SendText(display, text_x, text_y, "INSIDE") != 0) {
            fprintf(stderr, "Unable to send data\n");
        }
    }
    else {
        if (SendText(display, text_x, text_y, "OUTSIDE") != 0) {
            fprintf(stderr, "Unable to send data\n");
        }
    }
    /* Dinky way to keep text visible and clear when the window's full */
    text_y += 20;
    if (text_y > 500) {
      text_x += 70;
      if (text_x > 500) {
        text_x %= 500;
        /* repaint the display */
        ClearScreen(display);
        //ProcessEvent(display, ExposeEventType);
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
    
    MainLoop(display);
    
    CloseDisplay(display);
    return 0;
}

#include <hlib/hlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "fileview.h"

#define MAX_ICONS 25

char *current_path = NULL;

void set_path(char *path)
{
    if (current_path != NULL)
        free(current_path);
        
    current_path = strdup(path);
}

char *get_path()
{
    return current_path;
}

void change_directory(const char *path)
{
    if (chdir(path) != 0) {
        fprintf(stderr, "Unable to change directory to %s\n", path);
        perror(NULL);
    }
    
    set_path(".");
}

IconList *next_icon(IconList *head, char *text, void *data);

IconList *create_icons_from_directory(char *path)
{
    IconList *head = NULL;
    DIR *dir = NULL;
    struct dirent *curr = NULL;
    
    if ((dir = opendir(path)) == NULL) {
        return NULL;
    }
    
    while ((curr = readdir(dir)) != NULL) {
        head = next_icon(head, curr->d_name, strdup(curr->d_name));
    }
    
    closedir(dir);
    return head;
}

IconList *next_icon(IconList *head, char *text, void *data)
{
    IconList *newHead = NULL;
    int x_delta = 25 + ICON_WIDTH;
    int y_delta = 25 + ICON_HEIGHT;
    int x_start = 25;
    int y_start = 25;
    
    int x = x_start;
    int y = y_start;
    
    int last_x = (5 * x_delta);
        
    if (head != NULL) {
        x = head->icon->x;
        y = head->icon->y;
    }
    
    /* Check if icon goes to the right of the previous one */
    if (x + x_delta <= last_x && head != NULL) {
        x = head->icon->x + x_delta;
    } else if (head != NULL) {
        x = x_start;
        y = head->icon->y + y_delta;
    }
    
    newHead = iconlist_add(head, x, y, text, data);
    return newHead;
}


void draw_icon(Display *display, Icon *icon, IconText *text)
{
    
    DrawRectangle(display, 
                  icon->x, 
                  icon->y, 
                  icon->width, 
                  icon->height);
    
    /* Draw icon text */              
    SendText(display, text->x, text->y, text->label);
}

void draw_icons(Display *display, IconList *icons) 
{
    IconList *curr = NULL;
    
    curr = icons;

    while (curr != NULL) {
        draw_icon(display, curr->icon, curr->text);
        curr = curr->next;
    }
}

int is_directory(char *path)
{
    struct stat st_info;
    int status = 0;
    
    status = stat(path, &st_info);
    if (status != 0) {
        fprintf(stderr, "Error reading file %s\n", path);
        perror(NULL);
        return 0;
    }
    
    return S_ISDIR(st_info.st_mode);
}

/** Event handlers **/
void expose_event(Display *display, Event *event, void *data)
{
    FileView *fv = (FileView *) data;
    IconList *icons = NULL;
    char *path = get_path();
    
    if (is_directory(path)) {
        icons = create_icons_from_directory(path);
        fileview_set_icons(fv, icons);
        draw_icons(fv->display, fv->icons);
    } else {
        SendText(fv->display, 150, 150, path);
    }
    
}

void click_event(Display *display, Event *event, void *data)
{
    FileView *fv = (FileView *) data;
    IconList *icons = fv->icons;
    IconList *clicked = NULL;
        
    clicked = icon_at_position(icons, event->val.click.x, event->val.click.y);
    
    ClearScreen(display);
    
    /* Indicate which icon was clicked */
    if (clicked != NULL) {
        if (is_directory(clicked->data)) {
            change_directory(clicked->data);
        }
        else {
            set_path(clicked->data);
        }
    }
    
}


int main()
{
    Display *display = NULL;
    char *host = "localhost";
    int port = 9090;
    
    FileView *fileview = NULL;
    
    display = OpenDisplay("localhost", 9090);
    if (display == NULL) {
        fprintf(stderr, "Unable to connect to display %s:%d\n", host, port);
        exit(1);
    }
    
    set_path(".");
    fileview = fileview_new(display);
    
    /* Register Callbacks */
    RegisterCallback(display, ExposeEventType, expose_event, fileview);
    RegisterCallback(display, ClickEventType, click_event, fileview);
    
    MainLoop(display);
    
    CloseDisplay(display);
    return 0;
}

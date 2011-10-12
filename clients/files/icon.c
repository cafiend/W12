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

#include "icon.h"

#include <stdlib.h>
#include <string.h>

Icon *icon_new(int x, int y, int width, int height)
{
    Icon *icon = NULL;
    
    icon =  (Icon *) calloc(1, sizeof(Icon));
    
    icon->x = x;
    icon->y = y;
    icon->width = width;
    icon->height = height;
        
    return icon;
}

void icon_free(Icon *icon)
{
    if (icon == NULL)
        return;
        
    free(icon);
}

IconText *icon_text_new(char *text, int text_x, int text_y)
{
    IconText *itext = NULL;
    
    itext =  (IconText *) calloc(1, sizeof(IconText));
    
    itext->label = strdup(text);
    itext->x = text_x;
    itext->y = text_y;
        
    return itext;
}

void icon_text_free(IconText *itext)
{
    if (itext == NULL)
        return;
        
    if (itext->label != NULL)
        free(itext->label);
        
    free(itext);
    
    return;
}


IconList *iconlist_add(IconList *list, int x, int y, char *text, void *data)
{
    IconList *head = NULL;
    Icon *icon = NULL;
    IconText *itext = NULL;
    
    int itext_yoffset = 0;
    
    icon = icon_new(x, y, ICON_WIDTH, ICON_HEIGHT);
    head = (IconList *) calloc(1, sizeof(IconList));
    
    /* Calculate text position */
    itext_yoffset = y + ICON_HEIGHT + ICON_TEXT_OFFSET;
    
    itext = icon_text_new(text, x, itext_yoffset);
    
    head->icon = icon;
    head->text = itext;
    head->data = data;
    head->next = list;
    
    return head;
}

/* TODO: implement */
void iconlist_free(IconList *list)
{
    
}

static int is_in_icon(Icon *icon, int x, int y)
{
    int icon_right = 0;
    int icon_bottom = 0;
    
    if (icon == NULL)
        return 0;
    
    icon_right = icon->x + icon->width;
    icon_bottom = icon->y + icon->height;
    
    /* Check if inside horizontal boundaries */
    if (x >= icon->x && x <= icon_right) {
        /* Check if also inside vertical boundaries */
        if (y >= icon->y && y <= icon_bottom)
            return 1;
    }
    
    return 0;
}

IconList *icon_at_position(IconList *list, int x, int y)
{
    IconList *curr = NULL;
    
    curr = list;

    while (curr != NULL) {
        if (is_in_icon(curr->icon, x, y)) {
            return curr;
        }
        curr = curr->next;
    }
    
    return NULL;
}

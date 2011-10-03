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

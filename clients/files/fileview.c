#include <stdlib.h>

#include "fileview.h"

FileView *fileview_new(Display *display)
{
    FileView *fileview = NULL;
    
    fileview = (FileView *) calloc(1, sizeof(FileView));
    fileview->display = display;
    fileview->icons = NULL;
    
    return fileview;
}

void fileview_free(FileView *fileview)
{
    if (fileview == NULL)
        return;

    if (fileview->display != NULL)
        CloseDisplay(fileview->display);
       
    if (fileview->icons != NULL)
        iconlist_free(fileview->icons);

    free(fileview);
}

void fileview_set_icons(FileView *fileview, IconList *icons)
{
    if (fileview == NULL)
        return;
        
    if (fileview->icons != NULL)
        iconlist_free(fileview->icons);
        
    fileview->icons = icons;
}

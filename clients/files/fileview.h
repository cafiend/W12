#ifndef __FILEVIEW_H
#define __FILEVIEW_H

#include <hlib/hlib.h>
#include "icon.h"

typedef struct FileView {
    IconList *icons;
    Display *display;
} FileView;


FileView *fileview_new(Display *display);
void fileview_free(FileView *fileview);

void fileview_set_icons(FileView *fileview, IconList *icons);

#endif /* __FILEVIEW_H */

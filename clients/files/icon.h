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

#ifndef __ICON_H
#define __ICON_H

#define ICON_WIDTH 60
#define ICON_HEIGHT 50
#define ICON_TEXT_OFFSET 20

typedef struct Icon {
    int x; /* X coordinate of upper left corner */
    int y; /* Y coordinate of upper right corner */
    int width;
    int height;
} Icon;

typedef struct IconText {
    char *label; /* Text label for the icon */
    int x;
    int y;
} IconText;

typedef struct IconList {
    Icon *icon;
    IconText *text;
    void *data;  /* User data associated with this icon */
    struct IconList *next;
} IconList;

Icon *icon_new(int x, int y, int width, int height);
void icon_free(Icon *icon);

IconText *icon_text_new(char *text, int x, int y);
void icon_text_free(IconText *icontext);

IconList *iconlist_add(IconList *list, int x, int y, char *text, void *data);
IconList *icon_at_position(IconList *list, int x, int y);
void iconlist_free(IconList *list);

#endif /* __ICON_H */

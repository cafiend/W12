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

#ifndef _EVENT_H
#define _EVENT_H

typedef enum EventType
{
	SetupEventType,
    ExposeEventType,
    ClickEventType
} EventType;

/**
 * Event Structures
 */
typedef struct ClickEvent
{
    int x;  /* X coordinate of click */
    int y;  /* Y coordinate of click */
} ClickEvent;

/**
 * General Event structure
 */
typedef struct Event
{
    EventType type;
    union {
        ClickEvent click;
    } val;
} Event;

/** Event constructors */
void event_free(Event *e);
Event *event_expose_new();
Event *event_click_new(int x, int y);
Event *event_setup_new();
#endif

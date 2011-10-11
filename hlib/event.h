#ifndef _EVENT_H
#define _EVENT_H

typedef enum EventType
{
	SetupEventType,
    ExposeEventType,
    ClickEventType,
    MouseDownEventType,
    MouseMoveEventType,
    MouseDragEventType
} EventType;

/**
 * Event Structures
 */
typedef struct MouseEvent
{
    int x;  /* X coordinate of click */
    int y;  /* Y coordinate of click */
    int button; /* mouse button clicked */
    int dx;	/* mouse movement deltas */	
    int dy;
} MouseEvent;

/**
 * General Event structure
 */
typedef struct Event
{
    EventType type;
    union {
        MouseEvent mouse;
    } val;
} Event;

/** Event constructors */
void event_free(Event *e);
Event *event_expose_new();
Event *event_click_new(int x, int y, int button);
Event *event_mousedown_new(int x, int y, int button);
Event *event_mousemove_new(int x, int y, int dx, int dy);
Event *event_mousedrag_new(int x, int y, int dx, int dy, int button);
Event *event_setup_new();
#endif

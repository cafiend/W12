#include <stdlib.h>

#include "event.h"

#define NEW(type) (type *)calloc(1, sizeof(type))

void event_free(Event *e)
{
    if (e == NULL)
        return;
        
    free(e);
}


Event *event_expose_new()
{
    Event *e = NEW(Event);
    e->type = ExposeEventType;
    return e;
}

Event *event_setup_new()
{
    Event *e = NEW(Event);
    e->type = SetupEventType;
    return e;
}

Event *event_click_new(int x, int y, int button)
{
    Event *e = NEW(Event);
    e->type = ClickEventType;
    
    e->val.mouse.x = x;
    e->val.mouse.y = y;
    e->val.mouse.button = button;
    
    return e;
}

Event *event_mousedown_new(int x, int y, int button)
{
    Event *e = NEW(Event);
    e->type = MouseDownEventType;
    
    e->val.mouse.x = x;
    e->val.mouse.y = y;
    e->val.mouse.button = button;
    
    return e;
}

Event *event_mousemove_new(int x, int y, int dx, int dy)
{
    Event *e = NEW(Event);
    e->type = MouseMoveEventType;
    
    e->val.mouse.x = x;
    e->val.mouse.y = y;
    e->val.mouse.dx = dx;
	e->val.mouse.dy = dy;
    return e;
}

Event *event_mousedrag_new(int x, int y, int dx, int dy, int button)
{
    Event *e = NEW(Event);
    e->type = MouseDragEventType;
    
    e->val.mouse.x = x;
    e->val.mouse.y = y;
    e->val.mouse.dx = dx;
	e->val.mouse.dy = dy;
	e->val.mouse.button = button;
    return e;
}

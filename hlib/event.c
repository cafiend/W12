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

Event *event_click_new(int x, int y)
{
    Event *e = NEW(Event);
    e->type = ClickEventType;
    
    e->val.click.x = x;
    e->val.click.y = y;
    
    return e;
}

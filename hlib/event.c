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

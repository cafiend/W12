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
#include "callback.h"

/* Create and initialize a new CallbackList */
CallbackList *callbacklist_new()
{
    CallbackList *list = NULL;
    
    list = (CallbackList *) calloc(1, sizeof(CallbackList));
    
    return list;
}

/* Free an CallbackList */
void callbacklist_free(CallbackList *list)
{
    CallbackList *curr = NULL;
    CallbackList *tmp = NULL;
    
    curr = list;

    while (curr != NULL) {
        tmp = curr;
        curr = curr->next;
        
        free(tmp);
    }   
}

void callbacklist_call(CallbackList *list, struct Display *display, Event *event)
{
    CallbackList *curr = NULL;
    EventCallback cb = NULL;
    
    curr = list;

    while (curr != NULL) {
        cb = curr->cb;
        cb(display, event, curr->data);
        curr = curr->next;
    }
}

/* Add a new callback to the head of the linked list 
 * Returns the head of the list
 */
CallbackList *callbacklist_add(CallbackList *list, EventCallback cb, void *data)
{
    CallbackList *newHead = callbacklist_new();
    
    newHead->cb = cb;
    newHead->data = data;
    newHead->next = list;
    
    return newHead;
}

Callbacks *callbacks_new()
{
    Callbacks *cbs = NULL;
    
    cbs = (Callbacks *) calloc(1, sizeof(Callbacks));
    
    return cbs;
}

void callbacks_free(Callbacks *callbacks)
{
    if (callbacks == NULL)
        return;
        
    callbacklist_free(callbacks->clickHandlers);
    callbacklist_free(callbacks->mouseDownHandlers);
    callbacklist_free(callbacks->exposeHandlers);
    callbacklist_free(callbacks->setupHandlers);
    
    free(callbacks);
}

void callbacks_add(Callbacks *callbacks, EventType type, EventCallback cb,  void *data)
{
    if (callbacks == NULL)
        return;
       
    switch(type) {
        case ExposeEventType:
            callbacks->exposeHandlers	= callbacklist_add(callbacks->exposeHandlers, cb, data);
        break;
        case SetupEventType:
			callbacks->setupHandlers 	= callbacklist_add(callbacks->setupHandlers, cb, data);
		break;
        case ClickEventType:
             callbacks->clickHandlers 	= callbacklist_add(callbacks->clickHandlers, cb, data);
        break;
        case MDownEventType:
             callbacks->mouseDownHandlers 	= callbacklist_add(callbacks->mouseDownHandlers, cb, data);
        break;
        case MMoveEventType:
             callbacks->mouseMoveHandlers 	= callbacklist_add(callbacks->mouseMoveHandlers, cb, data);
        break;
        case MDragEventType:
             callbacks->mouseDragHandlers 	= callbacklist_add(callbacks->mouseDragHandlers, cb, data);
        break;
    }
    
    return;
}

void callbacks_call(Callbacks *callbacks, struct Display *display, Event *event)
{
    switch(event->type) {
        case ExposeEventType: 
            callbacklist_call(callbacks->exposeHandlers, display, event);
        break;
        case SetupEventType:
			callbacklist_call(callbacks->setupHandlers, display, event);
        break;
        case ClickEventType: 
            callbacklist_call(callbacks->clickHandlers, display, event);
        break;
        case MDownEventType: 
            callbacklist_call(callbacks->mouseDownHandlers, display, event);
        break;
        case MMoveEventType: 
            callbacklist_call(callbacks->mouseMoveHandlers, display, event);
        break;
        case MDragEventType: 
            callbacklist_call(callbacks->mouseDragHandlers, display, event);
        break;
    }
    
    event_free(event);
}

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
    callbacklist_free(callbacks->exposeHandlers);
    
    free(callbacks);
}

void callbacks_add(Callbacks *callbacks, EventType type, EventCallback cb,  void *data)
{
    if (callbacks == NULL)
        return;
       
    switch(type) {
        case ExposeEventType:
            callbacks->exposeHandlers = callbacklist_add(callbacks->exposeHandlers, cb, data);
        break;
        
        case ClickEventType:
             callbacks->clickHandlers = callbacklist_add(callbacks->clickHandlers, cb, data);
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
            
        case ClickEventType: 
            callbacklist_call(callbacks->clickHandlers, display, event);
        break;
    }
    
    event_free(event);
}

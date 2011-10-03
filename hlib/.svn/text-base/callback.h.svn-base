#ifndef __CALLBACK_H
#define __CALLBACK_H

#include "event.h"

struct Display;

typedef void (*EventCallback)(struct Display *d, Event *e, void *data);

typedef struct CallbackList {
    EventCallback cb;
    void *data;
    struct CallbackList *next;
} CallbackList;

typedef struct Callbacks {
    CallbackList *clickHandlers;
    CallbackList *exposeHandlers;
} Callbacks;

CallbackList *callbacklist_new();
void callbacklist_free(CallbackList *list);
CallbackList *callbacklist_add(CallbackList *list, EventCallback cb, void *data);
void callbacklist_call(CallbackList *list, struct Display *display, Event *event);

Callbacks *callbacks_new();
void callbacks_free(Callbacks *callbacks);
void callbacks_add(Callbacks *callbacks, EventType type, EventCallback cb,  void *data);
void callbacks_call(Callbacks *callbacks, struct Display *display, Event *event);


#endif /* __CALLBACK_H */

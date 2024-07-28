#include "event.h"

#include "core/vememory.h"

typedef struct registered_event
{
    void* listener;
    PFN_on_event callback;
} registered_event;

typedef struct event_code_entry 
{
    registered_event* events;
} event_code_entry;
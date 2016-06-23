#include "logicevent.h"
#include "SDL.h"

unsigned int LogicEvent::_eventType = 0;

LogicEvent::~LogicEvent()
{
    SDL_Event event;
    SDL_zero( event );
    event.type = _eventType;
    event.user.code = static_cast<int>(methodCode);
    SDL_PushEvent( &event );
}

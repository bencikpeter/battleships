#include "logicevent.h"
#include "SDL.h"

unsigned int LogicEvent::_eventType = 0;

unsigned int LogicEvent::getEventType()
{
    return _eventType;
}

LogicEvent::LogicEvent()
{

}

LogicEvent::~LogicEvent()
{
    SDL_Event event;
    SDL_zero( event );
    event.type = _eventType;
    SDL_PushEvent( &event );
}

void LogicEvent::setType(unsigned int eventType)
{
    _eventType = eventType;
}

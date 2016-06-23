#ifndef LOGICEVENT_H
#define LOGICEVENT_H

enum MethodCode { SEND_LAYOUT, GET_LAYOUT, SHOOTSEND, CONNECT, ENEMY_SHOT, HOST };

class LogicEvent
{
private:
    static unsigned int _eventType;
    MethodCode methodCode;

public:
    LogicEvent( MethodCode methodCode ):methodCode( methodCode )
    { }

    LogicEvent( const LogicEvent& ) = default;

    LogicEvent& operator=( const LogicEvent& ) = delete;

    ~LogicEvent();

    static void setType( unsigned int eventType )
    {
        _eventType = eventType;
    }

    static unsigned int getEventType()
    {
        return _eventType;
    }
};

#endif // LOGICEVENT_H

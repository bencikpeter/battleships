#ifndef LOGICEVENT_H
#define LOGICEVENT_H

enum MethodCode { SEND_LAYOUT, GET_LAYOUT, SHOOT, CONNECT, ENEMY_SHOT, HOST };


class LogicEvent
{
private:
    static unsigned int _eventType;
    MethodCode methodCode;

public:
    LogicEvent( MethodCode methodCode );

    ~LogicEvent();

    static void setType( unsigned int _eventType );

    static unsigned int getEventType();
};

#endif // LOGICEVENT_H

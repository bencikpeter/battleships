#ifndef LOGICEVENT_H
#define LOGICEVENT_H


class LogicEvent
{
private:
    static unsigned int _eventType;
public:
    LogicEvent();

    ~LogicEvent();

    static void setType( unsigned int _eventType );

    static unsigned int getEventType();
};

#endif // LOGICEVENT_H

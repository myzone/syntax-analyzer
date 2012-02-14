#pragma once

#include <QString>

#define DECLARE_SHARE_METHOD(class_name) \
    virtual void share(const EventBroadcaster& broadcaster) const;

#define DECLARE_TO_STRING_METHOD(class_name) \
    virtual const QString toString() const;

#define DECLARE_ERROR_EVENT_SUBCLASS(class_name) \
    class class_name : public ErrorEvent {\
    public:\
        class_name(const QString& symbol);\
        virtual ~class_name();\
        DECLARE_SHARE_METHOD(class_name);\
        DECLARE_TO_STRING_METHOD(class_name);\
    };

namespace Events {

    class EventBroadcaster;

    class Event {       
    public:
        DECLARE_SHARE_METHOD(Event);
        DECLARE_TO_STRING_METHOD(Event)
    };
   
    
    class ErrorEvent : public Event {
    private:
        QString symbol;
    public:
        ErrorEvent(const QString& symbol);
        virtual ~ErrorEvent();
        const QString& getSymbol() const;

        DECLARE_SHARE_METHOD(ErrorEvent);
        DECLARE_TO_STRING_METHOD(ErrorEvent);
    };

    DECLARE_ERROR_EVENT_SUBCLASS(FatalErrorEvent);
    DECLARE_ERROR_EVENT_SUBCLASS(WarningEvent);

    DECLARE_ERROR_EVENT_SUBCLASS(DoubleDefenitionErrorEvent);
    DECLARE_ERROR_EVENT_SUBCLASS(SymbolIsNotDefinedErrorEvent);
}
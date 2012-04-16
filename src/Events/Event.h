#pragma once

#include <QString>
#include <QThread>

#include "../defines.h"

#define DECLARE_SHARE_METHOD(class_name) \
    virtual void share(EventBroadcaster& broadcaster);

#define DECLARE_TO_STRING_METHOD(class_name) \
    virtual const QString toString() const;

#define DECLARE_EVENT_SUBCLASS(class_name) \
    class class_name : public Event {\
    public:\
        class_name();\
        virtual ~class_name();\
        DECLARE_SHARE_METHOD(class_name);\
        DECLARE_TO_STRING_METHOD(class_name);\
    };

#define DECLARE_ERROR_EVENT_SUBCLASS(class_name) \
    class class_name : public ErrorEvent {\
    public:\
        class_name();\
        class_name(const QString& symbol);\
        virtual ~class_name();\
        DECLARE_SHARE_METHOD(class_name);\
        DECLARE_TO_STRING_METHOD(class_name);\
    };

namespace Events {

    class EventBroadcaster;

    class Event {       
    public:
        Event();
        virtual ~Event();
        DECLARE_SHARE_METHOD(Event);
        DECLARE_TO_STRING_METHOD(Event)
    };
   
    
    class ErrorEvent : public Event {
    private:
        QString representation;
    public:
        ErrorEvent(const QString& symbol);
        virtual ~ErrorEvent();
        const QString& getRepresentation() const;

        DECLARE_SHARE_METHOD(ErrorEvent);
        DECLARE_TO_STRING_METHOD(ErrorEvent);
    };

    DECLARE_EVENT_SUBCLASS(AnalysingWasStartedEvent);
    DECLARE_EVENT_SUBCLASS(AnalysingWasEndedEvent);
    
    DECLARE_ERROR_EVENT_SUBCLASS(LibraryFileCannotBeFoundErrorEvent);
    DECLARE_ERROR_EVENT_SUBCLASS(LibraryFileHasMistakeErrorEvent);

    DECLARE_ERROR_EVENT_SUBCLASS(DoubleDefenitionErrorEvent);
    DECLARE_ERROR_EVENT_SUBCLASS(SymbolIsNotUsedErrorEvent);
    DECLARE_ERROR_EVENT_SUBCLASS(SymbolIsNotDefinedErrorEvent);
    DECLARE_ERROR_EVENT_SUBCLASS(SymbolHasMistakeErrorEvent);
    DECLARE_ERROR_EVENT_SUBCLASS(WrongSymbolDefinitionErrorEvent);
    
    DECLARE_ERROR_EVENT_SUBCLASS(SymbolIsNotClosedErrorEvent);
    DECLARE_ERROR_EVENT_SUBCLASS(WrongBracketsNumberErrorEvent);
}
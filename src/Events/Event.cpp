#include "../Events/Event.h"
#include "../Events/EventBroadcaster.h"

#define IMPLEMENT_SHARE_METHOD(class_name) \
    void class_name::share(EventBroadcaster& broadcaster) {\
        broadcaster.broadcast<class_name>(*this);\
    }

#define IMPLEMENT_TO_STRING_METHOD(class_name) \
    const QString class_name::toString() const {\
        return #class_name;\
    }

#define IMPLEMENT_EVENT_SUBCLASS(class_name) \
        class_name::class_name() : Event() {}\
        class_name::~class_name() {}\
        IMPLEMENT_SHARE_METHOD(class_name);\
        IMPLEMENT_TO_STRING_METHOD(class_name);

#define IMPLEMENT_ERROR_EVENT_SUBCLASS(class_name) \
        class_name::class_name() : ErrorEvent("") {}\
        class_name::class_name(const QString& symbol) : ErrorEvent(symbol) {}\
        class_name::~class_name() {}\
        IMPLEMENT_SHARE_METHOD(class_name);\
        IMPLEMENT_TO_STRING_METHOD(class_name);

namespace Events {

    Event::Event() {
    }
    
    Event::~Event() {
    }
    
    IMPLEMENT_SHARE_METHOD(Event);
    IMPLEMENT_TO_STRING_METHOD(Event);

    ErrorEvent::ErrorEvent(const QString& symbol) : representation(symbol) {
    }

    ErrorEvent::~ErrorEvent() {
    }

    const QString& ErrorEvent::getRepresentation() const {
        return representation;
    };

    IMPLEMENT_SHARE_METHOD(ErrorEvent);
    IMPLEMENT_TO_STRING_METHOD(ErrorEvent);

    IMPLEMENT_EVENT_SUBCLASS(AnalysingWasStartedEvent);
    IMPLEMENT_EVENT_SUBCLASS(AnalysingWasEndedEvent);

    IMPLEMENT_ERROR_EVENT_SUBCLASS(LibraryFileCannotBeFoundErrorEvent);
    IMPLEMENT_ERROR_EVENT_SUBCLASS(LibraryFileHasMistakeErrorEvent);

    IMPLEMENT_ERROR_EVENT_SUBCLASS(DoubleDefenitionErrorEvent);
    IMPLEMENT_ERROR_EVENT_SUBCLASS(SymbolIsNotUsedErrorEvent);
    IMPLEMENT_ERROR_EVENT_SUBCLASS(SymbolIsNotDefinedErrorEvent);
    IMPLEMENT_ERROR_EVENT_SUBCLASS(SymbolHasMistakeErrorEvent);
    IMPLEMENT_ERROR_EVENT_SUBCLASS(WrongSymbolDefinitionErrorEvent);
    
    IMPLEMENT_ERROR_EVENT_SUBCLASS(SymbolIsNotClosedErrorEvent);
    IMPLEMENT_ERROR_EVENT_SUBCLASS(WrongBracketsNumberErrorEvent);
}
#include "ErrorEvent.h"

#include "ErrorEventBroadcaster.h"

#define IMPLEMENT_SHARE_METHOD(class_name) \
    void class_name::share(const ErrorEventBroadcaster& broadcaster) const {\
        broadcaster.broadcast<class_name>(*this);\
    }

#define IMPLEMENT_ERROR_EVENT_SUBCLASS(class_name) \
        class_name::class_name(const QString& symbol) : ErrorEvent(symbol) {}\
        class_name::~class_name() {}\
        IMPLEMENT_SHARE_METHOD(class_name);

ErrorEvent::ErrorEvent(const QString& symbol) {
    this->symbol = symbol;
}
ErrorEvent::~ErrorEvent() {
    
}
const QString& ErrorEvent::getSymbol() const {
    return symbol;
};

IMPLEMENT_SHARE_METHOD(ErrorEvent);

IMPLEMENT_ERROR_EVENT_SUBCLASS(FatalErrorEvent);
IMPLEMENT_ERROR_EVENT_SUBCLASS(WarningEvent);

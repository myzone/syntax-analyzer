#include "ErrorEventBroadcaster.h"

ErrorEventBroadcaster::ErrorEventBroadcaster() {
}

ErrorEventBroadcaster::~ErrorEventBroadcaster() {
}

void ErrorEventBroadcaster::addErrorEventListener(const ErrorEventListener& listener) {
    listeners.push_back(listener);
}

void ErrorEventBroadcaster::removeErrorEventListener(const ErrorEventListener& listener) {
    listeners.removeOne(listener);
}

template<typename T> void ErrorEventBroadcaster::broadcast(T event) const {
    for (QLinkedList<ErrorEventListener>::Iterator it = listeners.begin(); it != listeners.end(); it++) {
        static_cast<ErrorEventListener> (it).handle(event);
    }
}


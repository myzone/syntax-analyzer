#include "EventBroadcaster.h"

namespace Events {

    EventBroadcaster::EventBroadcaster() {
    }

    EventBroadcaster::~EventBroadcaster() {
    }

    void EventBroadcaster::addErrorEventListener(EventListener* listener) {
        listeners.push_back(listener);
    }

    void EventBroadcaster::removeErrorEventListener(EventListener* listener) {
        listeners.removeOne(listener);
    }

}
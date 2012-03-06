#include "EventBroadcaster.h"

namespace Events {

    EventBroadcaster::EventBroadcaster() {
    }

    EventBroadcaster::~EventBroadcaster() {
    }

    void EventBroadcaster::addEventListener(EventListener* listener) {
        listeners.push_back(listener);
    }

    void EventBroadcaster::removeEventListener(EventListener* listener) {
        listeners.removeOne(listener);
    }

}
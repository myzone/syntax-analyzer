#include "EventBroadcaster.h"

namespace Events {

    EventBroadcaster::EventBroadcaster() {
    }

    EventBroadcaster::~EventBroadcaster() {
    }

    void EventBroadcaster::addEventListener(EventListener* listener) {
        listeners.append(listener);
    }

    void EventBroadcaster::removeEventListener(EventListener* listener) {
        listeners.removeOne(listener);
    }

}
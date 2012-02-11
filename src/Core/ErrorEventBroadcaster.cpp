#include "ErrorEventBroadcaster.h"

namespace Core {

    ErrorEventBroadcaster::ErrorEventBroadcaster() {
    }

    ErrorEventBroadcaster::~ErrorEventBroadcaster() {
    }

    void ErrorEventBroadcaster::addErrorEventListener(ErrorEventListener* listener) {
        listeners.push_back(listener);
    }

    void ErrorEventBroadcaster::removeErrorEventListener(ErrorEventListener* listener) {
        listeners.removeOne(listener);
    }

}
#pragma once

#include <QtCore>
#include <QLinkedList>

#include "EventListener.h"

namespace Events {

    class EventBroadcaster {
    private:
        QLinkedList<EventListener*> listeners;
    public:
        EventBroadcaster();
        virtual ~EventBroadcaster();

        void addEventListener(EventListener* listener);
        void removeEventListener(EventListener* listener);

        template<typename T> void broadcast(const T& event) const {
            for (QLinkedList<EventListener*>::ConstIterator it = listeners.begin(); it != listeners.end(); it++) {
                (*it)->handle(event);
            }
        }
    };
}
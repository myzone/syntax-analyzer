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

        void addErrorEventListener(EventListener* listener);
        void removeErrorEventListener(EventListener* listener);

        template<typename T> void broadcast(T event) const {
            for (QLinkedList<EventListener*>::ConstIterator it = listeners.begin(); it != listeners.end(); it++) {
                (*it)->handle(event);
            }
        }
    };
}
#pragma once

#include <QtCore>
#include <QLinkedList>

#include "EventListener.h"

namespace Events {

    class EventBroadcaster {
    private:
        QList<EventListener*> listeners;
    public:
        EventBroadcaster();
        virtual ~EventBroadcaster();

        void addEventListener(EventListener* listener);
        void removeEventListener(EventListener* listener);

        template<typename T> void broadcast(const T& event) {
            //for (QList<EventListener*>::Iterator it = listeners.begin(), end = listeners.end(); it != end; ++it) {
            //    (*it)->handle(event);
            //}
            for (int i = 0; i < listeners.count(); i++) {
                listeners[i]->handle(event);
            }

        }
    };
}
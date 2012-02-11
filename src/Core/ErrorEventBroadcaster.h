#pragma once

#include <QtCore>
#include <QLinkedList>

#include "ErrorEventListener.h"

namespace Core {

    class ErrorEventBroadcaster {
    private:
        QLinkedList<ErrorEventListener*> listeners;
    public:
        ErrorEventBroadcaster();
        virtual ~ErrorEventBroadcaster();

        void addErrorEventListener(ErrorEventListener* listener);
        void removeErrorEventListener(ErrorEventListener* listener);

        template<typename T> void broadcast(T event) const {
            for (QLinkedList<ErrorEventListener*>::ConstIterator it = listeners.begin(); it != listeners.end(); it++) {
                (*it)->handle(event);
            }
        }
    };
}
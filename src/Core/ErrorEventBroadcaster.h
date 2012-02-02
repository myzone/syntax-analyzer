#pragma once

#include <QLinkedList>

#include "ErrorEventListener.h"

class ErrorEventBroadcaster {
private:
    QLinkedList<ErrorEventListener> listeners;
public:
    ErrorEventBroadcaster();
    virtual ~ErrorEventBroadcaster();

    void addErrorEventListener(const ErrorEventListener& listener);
    void removeErrorEventListener(const ErrorEventListener& listener);
    template<typename T> void broadcast(T event) const;
};

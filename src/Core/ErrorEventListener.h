#pragma once

#include "ErrorEvent.h"

class ErrorEventListener {
public:
    ErrorEventListener();
    virtual ~ErrorEventListener();

    virtual void handle(const FatalErrorEvent& event) {
    }

    virtual void handle(const WarningEvent& event) {
    }

    virtual void handle(const ErrorEvent& event) {
    }
};

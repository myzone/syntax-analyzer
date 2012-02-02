#pragma once

#include "ErrorEvent.h"

#define pure =0

class ErrorEventListener {
public:
    ErrorEventListener();
    virtual ~ErrorEventListener();

    virtual void handle(FatalErrorEvent event) {
    };

    virtual void handle(WarningEvent event) {
    };

    bool operator==(const ErrorEventListener& that) const;
};

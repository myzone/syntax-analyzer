#pragma once

#include <QString>

#include "ErrorEventBroadcaster.h"

class TextValidator {
public:
    TextValidator();
    virtual ~TextValidator();
    
    void validate(const QString& string, const ErrorEventBroadcaster& broadcaster) const;
};


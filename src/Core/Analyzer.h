#pragma once

#include <QString>

#include "ErrorEventBroadcaster.h"

class Analyzer {
private:
    ErrorEventBroadcaster broadcaster;
public:
    Analyzer();
    virtual ~Analyzer();

    void analyze(const QString& string) const;

    const ErrorEventBroadcaster& getErrorEventBroadcaster() const;
    void addErrorEventListener(const ErrorEventListener& listener);
    void removeErrorEventListener(const ErrorEventListener& listener);
};


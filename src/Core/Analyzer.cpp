#include "Analyzer.h"

Analyzer::Analyzer() {
    this->broadcaster = ErrorEventBroadcaster();
}

Analyzer::~Analyzer() {
}

const ErrorEventBroadcaster& Analyzer::getErrorEventBroadcaster() const {
    return broadcaster;
}

void Analyzer::addErrorEventListener(ErrorEventListener* listener) {
    broadcaster.addErrorEventListener(listener);
}

void Analyzer::removeErrorEventListener(ErrorEventListener* listener) {
    broadcaster.removeErrorEventListener(listener); 
}

void Analyzer::analyze(const QString& string) const {
    
}

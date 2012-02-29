#include "Analyzer.h"
#include "Exeption.h"

namespace Core {

    Analyzer::Analyzer(const QString& pathToLibrary) : broadcaster(), preprocessor(pathToLibrary), syntaxTreeFactory(&this->broadcaster) {
    }

    Analyzer::~Analyzer() {
    }

    void Analyzer::analyze(const QString& text) const {
        try {

        } catch (AnalyzeCrashExeption) {
        }
    }

    const Events::EventBroadcaster& Analyzer::getErrorEventBroadcaster() const {
        return broadcaster;
    }

    void Analyzer::addErrorEventListener(Events::EventListener* listener) {
        broadcaster.addErrorEventListener(listener);
    }

    void Analyzer::removeErrorEventListener(Events::EventListener* listener) {
        broadcaster.removeErrorEventListener(listener);
    }

}
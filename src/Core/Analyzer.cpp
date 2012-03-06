#include "Analyzer.h"
#include "Exeption.h"

namespace Core {

    Analyzer::Analyzer(const QString& pathToLibrary) : broadcaster(), preprocessor(pathToLibrary), syntaxTreeFactory(&this->broadcaster) { }

    Analyzer::~Analyzer() { }

    void Analyzer::analyze(const QString& text) const {
        shareStartEvent();

        try {
            QString source = preprocessor.process(text);
            Tree<Symbol> tree = syntaxTreeFactory.createTree(source);
        } catch (AnalyzeCrashExeption) {
            shareEndEvent();
        }

        shareEndEvent();
    }

    const Events::EventBroadcaster& Analyzer::getErrorEventBroadcaster() const {
        return broadcaster;
    }

    void Analyzer::addErrorEventListener(Events::EventListener* listener) {
        broadcaster.addEventListener(listener);
    }

    void Analyzer::removeErrorEventListener(Events::EventListener* listener) {
        broadcaster.removeEventListener(listener);
    }

    void Analyzer::shareStartEvent() const {
        Events::AnalysingWasStartedEvent event = Events::AnalysingWasStartedEvent();
        event.share(broadcaster);
    }

    void Analyzer::shareEndEvent() const {
        Events::AnalysingWasEndedEvent event = Events::AnalysingWasEndedEvent();
        event.share(broadcaster);
    }
}
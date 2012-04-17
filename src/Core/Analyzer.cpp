#include "Analyzer.h"
#include "Exeption.h"
#include "TreeAnalyzer.h"
#include "qwindowdefs.h"

namespace Core {

    Analyzer::Analyzer() : broadcaster(),
    preprocessor(&broadcaster, ""),
    syntaxTreeFactory(&broadcaster),
    treeAnalyzer(&broadcaster) { }
    
    Analyzer::Analyzer(const QString& pathToLibrary) : broadcaster(),
    preprocessor(&broadcaster, pathToLibrary),
    syntaxTreeFactory(&broadcaster),
    treeAnalyzer(&broadcaster) { }

    Analyzer::~Analyzer() { }

    void Analyzer::analyze(const QString& text) const {
        shareStartEvent();

        try {
            treeAnalyzer.analyzeTree(syntaxTreeFactory.createTree(preprocessor.process(text)));
        } catch (AnalyzeCrashExeption e) {
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
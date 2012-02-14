#include "Analyzer.h"
#include "Exeption.h"

namespace Core {

    
    
    Analyzer::Analyzer() {
        broadcaster = Events::EventBroadcaster();
    }

    Analyzer::Analyzer(const Analyzer& that) {
        broadcaster = that.broadcaster;
    }
    
    Analyzer::~Analyzer() {
        
    }
    
    void Analyzer::analyze(const QString& text) {
        try {
            
        } catch(AnalyzeCrashExeption) {
            Events::Event event = Events::Event();
            
            event.share(broadcaster);
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
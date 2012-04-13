#pragma once

#include "../Events/EventBroadcaster.h"
#include "../Core/Preprocessor.h"
#include "../Core/SyntaxTreeFactory.h"
#include "../Core/TreeAnalyzer.h"

#include "../defines.h"

namespace Core {

    class Analyzer {
    private:
        mutable Events::EventBroadcaster broadcaster;
        Preprocessor preprocessor;
        SyntaxTreeFactory syntaxTreeFactory;
        TreeAnalyzer treeAnalyzer;
    public:
        Analyzer();
        Analyzer(const QString& pathToLibrary);
        virtual ~Analyzer();

        void analyze(const QString& text) const;

        const Events::EventBroadcaster& getErrorEventBroadcaster() const;
        void addErrorEventListener(Events::EventListener* listener);
        void removeErrorEventListener(Events::EventListener* listener);
        
    private:
        void shareStartEvent() const;
        void shareEndEvent() const;
    };

}
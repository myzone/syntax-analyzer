#pragma once

#include "../Events/EventBroadcaster.h"

#define throws throw

namespace Core {

    class Analyzer {
    private:
        Events::EventBroadcaster broadcaster;

    public:
        Analyzer();
        Analyzer(const Analyzer& orig);
        virtual ~Analyzer();

        void analyze(const QString& text);

        const Events::EventBroadcaster& getErrorEventBroadcaster() const;
        void addErrorEventListener(Events::EventListener* listener);
        void removeErrorEventListener(Events::EventListener* listener);
    };

}
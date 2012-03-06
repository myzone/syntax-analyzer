#pragma once

#include <QString>

#include "../Utils/Tree.h"
#include "../Events/EventBroadcaster.h"

namespace Core {

    class TreeAnalyzer {
    private:
        const Events::EventBroadcaster* broadcaster;

    public:
        TreeAnalyzer(const Events::EventBroadcaster* broadcaster);
        virtual ~TreeAnalyzer();

    //   void analyzeTree(const Tree<Symbol>& tree) const;
    };

}


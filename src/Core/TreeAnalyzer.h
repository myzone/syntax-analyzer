#pragma once

#include <QString>

#include "../Utils/Tree.h"
#include "../Events/EventBroadcaster.h"

namespace Core {

    class TreeAnalyzer {
    public:
        TreeAnalyzer();
        virtual ~TreeAnalyzer();

        void analyzeTree(const Tree<QString>& tree, const Events::EventBroadcaster& broadcaster) const;
    };

}


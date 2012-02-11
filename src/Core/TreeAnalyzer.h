#pragma once

#include <QString>

#include "ErrorEventBroadcaster.h"
#include "Tree.h"

namespace Core {

    class TreeAnalyzer {
    public:
        TreeAnalyzer();
        virtual ~TreeAnalyzer();

        void analyzeTree(const Tree<QString>& tree, const ErrorEventBroadcaster& broadcaster) const;
    };

}


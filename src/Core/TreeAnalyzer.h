#pragma once

#include "SyntaxTree.h"
#include "ErrorEventBroadcaster.h"

class TreeAnalyzer {
public:
    TreeAnalyzer();
    virtual ~TreeAnalyzer();
    
    void analyzeTree(const SyntaxTree& tree, const ErrorEventBroadcaster& broadcaster) const;
};


#include <QStack>

#include "TreeAnalyzer.h"
#include "SyntaxTreeFactory.h"



namespace Core {

    TreeAnalyzer::TreeAnalyzer(const Events::EventBroadcaster* broadcaster) : broadcaster(broadcaster) { }

    TreeAnalyzer::~TreeAnalyzer() { }

    /*void TreeAnalyzer::analyzeTree(const Tree<Symbol>& tree) const {
        QList<Tree<Symbol> > list = QList<Tree<Symbol>>();
    }*/
}
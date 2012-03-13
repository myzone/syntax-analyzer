#include <QStack>

#include "TreeAnalyzer.h"
#include "SyntaxTreeFactory.h"



namespace Core {

    TreeAnalyzer::TreeAnalyzer(const Events::EventBroadcaster* broadcaster) : broadcaster(broadcaster) { }

    TreeAnalyzer::~TreeAnalyzer() { }

    void TreeAnalyzer::analyzeTree(const Tree<Symbol>& tree) const {
        TreeConverter treeConverter;
        tree.traverse(treeConverter);
        QList<Tree<Symbol> > nodes = treeConverter.getNodes();

        QMap<QString, bool> cache = QMap<QString, bool>();
        QList<Symbol> wrongSymbols = QList<Symbol > ();
        
        for (QList<Tree<Symbol> >::Iterator it = nodes.begin(); it != nodes.end(); ++it) {
            if ((*it).isEmpty()) continue;

            QList<bool> args = QList<bool>();

            if (!(*it).isLeaf()) {
                for (unsigned int i = 0; i < (*it).get().getType().getArgsNumber(); i++) {
                    args.append(cache[(*it)[i].get().getId()]);
                }
            }

            bool result = (*it).get().operation(args);
            cache.insert((*it).get().getId(), result);

            std::cout << (*it).get().getId().toStdString();
            if (result) {
                std::cout << "[style=filled,color=green]\n";
            } else {
                std::cout << "[style=filled,color=red]\n";
            }

            if (!(*it).isLeaf()
                    && (*it).get().getType() == Symbol::SymbolType::IDENTYFIER
                    && !result) {
                if (!wrongSymbols.contains((*it).get())) {
                    wrongSymbols.append((*it).get());
                }
                std::cout << (*it).get().toString().toStdString() << "\n";
            }
        }

        for (QList<Symbol>::ConstIterator it = wrongSymbols.begin(); it != wrongSymbols.end(); ++it) {
            Events::SymbolHasMistakeErrorEvent event = Events::SymbolHasMistakeErrorEvent((*it).getId());
            event.share(*broadcaster);

            std::cout << (*it).toString().toStdString() << "\n";
        }

    }

}
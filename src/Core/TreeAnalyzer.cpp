#include <QStack>

#include "TreeAnalyzer.h"
#include "SyntaxTreeFactory.h"



namespace Core {

    TreeAnalyzer::TreeAnalyzer(Events::EventBroadcaster* broadcaster) : broadcaster(broadcaster) { }

    TreeAnalyzer::~TreeAnalyzer() { }

    void TreeAnalyzer::analyzeTree(const Tree<Symbol>& tree) const throws(AnalyzeCrashExeption) {
        TreeConverter treeConverter;
        tree.traverse(treeConverter);
        QList<Tree<Symbol> > nodes = treeConverter.getNodes();

        QMap<QString, bool> cache = QMap<QString, bool>();
        QMap<QString, bool> wrongSymbols = QMap<QString, bool>();

        for (QList<Tree<Symbol> >::Iterator it = nodes.begin(); it != nodes.end(); ++it) {
            if (it->isEmpty()) continue;

            QList<bool> args = QList<bool>();

            if (!it->isLeaf()) {
                for (unsigned int i = 0; i < it->get().getType().getArgsNumber(); i++) {
                    try {
                        args.append(cache[(*it)[i].get().getId()]);
                    } catch (Tree<Symbol>::OutOfBoundsExeption exeption) {
                        Events::SymbolHasMistakeErrorEvent event = Events::SymbolHasMistakeErrorEvent((*it).get().getRepresentation());
                        event.share(*broadcaster);
                        
                        throw AnalyzeCrashExeption();
                    }
                }
            }

            bool result = (*it).get().operation(args);
            cache.insert((*it).get().getId(), result);

            if (wrongSymbols.contains((*it).get().getRepresentation())) {
                wrongSymbols[(*it).get().getRepresentation()] |= result;
            } else {
                wrongSymbols.insert((*it).get().getRepresentation(), result);
            }
        }

        for (QMap < QString, bool>::ConstIterator it = wrongSymbols.constBegin(), end = wrongSymbols.constEnd(); it != end; ++it) {
            if (!*it) {
                Events::SymbolHasMistakeErrorEvent event = Events::SymbolHasMistakeErrorEvent(it.key());
                event.share(*broadcaster);
            }
        }

    }

}
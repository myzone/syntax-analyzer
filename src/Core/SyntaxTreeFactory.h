#pragma once

#include <QString>

#include "../Utils/Tree.h"
#include "../Events/EventBroadcaster.h"
#include "../Core/Exeption.h"
#include "../Core/Symbol.h"

#include "../defines.h"

#include <iostream>

namespace Core {

    class SyntaxTreeFactory {
    private:
        Events::EventBroadcaster* broadcaster;

        SyntaxTreeFactory() { }

    public:
        SyntaxTreeFactory(Events::EventBroadcaster* broadcaster);
        Tree<Symbol> createTree(const QList<Symbol>& text) const throws(AnalyzeCrashExeption);

    private:
        QMap<QString, QList<Symbol> > createLinesMap(const QList<Symbol>& text) const throws(AnalyzeCrashExeption);
        QSet<QString> createLinesSet(const QMap<QString, QList<Symbol> >& map) const;
        void processLine(const QList<Symbol>& line, Tree<Symbol> tree) const throws(AnalyzeCrashExeption);
        QList<Symbol> toPostfixSymbolsList(const QList<Symbol>& line) const throws(AnalyzeCrashExeption);
        
        class TreeProcessor : public Tree<Symbol>::DataProcessor {
        private:
            mutable QList<Tree<Symbol> > nodes;
        public:

            TreeProcessor() : nodes() {
            }

            virtual void dataProcessingStarts() const {
                nodes.clear();
            }

            virtual void processData(Tree<Symbol>& nodeProvider) throws(Tree<Symbol>::TraverseStoppedExeption) {
                if (!nodeProvider.isLeaf()) return;

                if (nodeProvider.get().getType() == Symbol::SymbolType::IDENTYFIER) {
                    if (!nodeProvider.isEmpty() && !containsInSupertree(nodeProvider)) {
                        nodes.append(nodeProvider);
                    }
                }
            }

            virtual const TraverseType& getTraverseType() const {
                return TraverseType::WIDTH_TRAVERSE;
            }

            const QList<Tree<Symbol> >& getNodes() const {
                return nodes;
            }

            bool isEnd() const {
                return nodes.isEmpty();
            }
        private:

            static bool containsInSupertree(const Tree<Symbol>& tree) {
                if (tree.isRoot()) return false;

                Symbol symbol = tree.get();
                Tree<Symbol> current = tree.getSupertree();

                while (!current.isRoot()) {
                    if (!current.isEmpty()) {
                        if (current.get().getRepresentation() == symbol.getRepresentation()) {
                            return true;
                        }
                    }

                    current = current.getSupertree();
                }

                return false;
            }

        };
    };
}

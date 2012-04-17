#include <QRegExp>
#include <QLocale>
#include <QString>

#include "../Events/Event.h"
#include "../Core/Analyzer.h"
#include "../Core/SyntaxTreeFactory.h"
#include "../Core/Preprocessor.h"
#include "qwindowdefs.h"

namespace Core {

    SyntaxTreeFactory::SyntaxTreeFactory(Events::EventBroadcaster* broacaster) : broadcaster(broacaster) { }

    Tree<Symbol> SyntaxTreeFactory::createTree(const QList<Symbol>& text) const throws(AnalyzeCrashExeption) {
        const QString MAIN_SYMBOL = "main";

        QMap<QString, QList<Symbol> > linesMap = createLinesMap(text);

        QSet<QString> notUsedSymbolsSet = createLinesSet(linesMap);
        QSet<QString> notDefinedSymbolsSet = QSet<QString> ();

        Tree<Symbol> tree = Tree<Symbol > ();

        if (!linesMap.contains(MAIN_SYMBOL)) {
            Events::SymbolIsNotDefinedErrorEvent event = Events::SymbolIsNotDefinedErrorEvent(MAIN_SYMBOL);
            event.share(*broadcaster);

            throw AnalyzeCrashExeption();
        }

        QMap<QString, QList<Symbol> >::Iterator it = linesMap.find(MAIN_SYMBOL);
        QList<Symbol> value = *it;
        notUsedSymbolsSet.remove(MAIN_SYMBOL);

        tree = Symbol(MAIN_SYMBOL, Symbol::SymbolType::IDENTYFIER);
        tree.get().setId("a");

        TreeProcessor processor = TreeProcessor();

        bool ok = true;
        while (ok) {
            tree.traverse(processor);
            
            if (processor.isEnd())
                break;
 
            QList<Tree<Symbol> > nodes = processor.getNodes();

            for (QList<Tree<Symbol> >::ConstIterator it = nodes.constBegin(), end = nodes.constEnd(); it != end; ++it) {
                QString current = it->get().getRepresentation();
                notUsedSymbolsSet.remove(current);

                if (!linesMap.contains(current)) {
                    notDefinedSymbolsSet.insert(current);

                    ok = false;
                    continue;
                }

                processLine(linesMap.value(current), *it);
            }

        }

        for (QSet<QString>::ConstIterator it = notDefinedSymbolsSet.constBegin(); it != notDefinedSymbolsSet.constEnd(); ++it) {
            Events::SymbolIsNotDefinedErrorEvent event = Events::SymbolIsNotDefinedErrorEvent(*it);
            event.share(*broadcaster);
        }

        for (QSet<QString>::ConstIterator it = notUsedSymbolsSet.constBegin(); it != notUsedSymbolsSet.constEnd(); ++it) {
            Events::SymbolIsNotUsedErrorEvent event = Events::SymbolIsNotUsedErrorEvent(*it);
            event.share(*broadcaster);
        }

        if (!ok) throw AnalyzeCrashExeption();

        return tree;
    }

    QMap<QString, QList<Symbol> > SyntaxTreeFactory::createLinesMap(const QList<Symbol>& text) const throws(AnalyzeCrashExeption) {
        QMap<QString, QList<Symbol> > linesMap = QMap<QString, QList<Symbol> >();

        for (QList<Symbol>::ConstIterator it = text.constBegin(), end = text.constEnd(); it != end; ++it) {
            Symbol key = *(it++);
            QList<Symbol> defineList;

            if (key.getType() == Symbol::SymbolType::LITHERAL) {
                Events::WrongSymbolDefinitionErrorEvent wrongKeyEvents = Events::WrongSymbolDefinitionErrorEvent(key.getRepresentation());
                wrongKeyEvents.share(*broadcaster);

                throw AnalyzeCrashExeption();
            }

            if (it == end) {
                return linesMap;
            }

            Symbol defineSymbol = (*(it++));
            if (defineSymbol.getType() != Symbol::SymbolType::DEFINE) {
                Events::WrongSymbolDefinitionErrorEvent wrongKeyEvents = Events::WrongSymbolDefinitionErrorEvent(key.getRepresentation());
                Events::WrongSymbolDefinitionErrorEvent wrongDefEvents = Events::WrongSymbolDefinitionErrorEvent(defineSymbol.getRepresentation());

                wrongKeyEvents.share(*broadcaster);
                wrongDefEvents.share(*broadcaster);

                throw AnalyzeCrashExeption();
            }

            while (true) {
                if (it == end)
                    return linesMap;

                if (it->getType() == Symbol::SymbolType::DEFINE_END)
                    break;

                if (it->getType() == Symbol::SymbolType::DEFINE) {
                    Events::WrongSymbolDefinitionErrorEvent wrongKeyEvent = Events::WrongSymbolDefinitionErrorEvent(key.getRepresentation());
                    wrongKeyEvent.share(*broadcaster);

                    for (QList<Symbol>::ConstIterator it = defineList.constBegin(), end = defineList.constEnd(); it != end; ++it) {
                        Events::WrongSymbolDefinitionErrorEvent wrongDefEvent = Events::WrongSymbolDefinitionErrorEvent(it->getRepresentation());
                        wrongDefEvent.share(*broadcaster);
                    }

                    throw AnalyzeCrashExeption();
                }

                defineList += *(it++);
            }

            if (linesMap.contains(key.getRepresentation())) {
                Events::DoubleDefenitionErrorEvent event = Events::DoubleDefenitionErrorEvent(key.getRepresentation());
                event.share(*broadcaster);

                throw AnalyzeCrashExeption();
            }

            linesMap.insert(key.getRepresentation(), toPostfixSymbolsList(defineList));
        }

        return linesMap;
    }

    QSet<QString> SyntaxTreeFactory::createLinesSet(const QMap<QString, QList<Symbol> >& map) const {
        QSet<QString> set = QSet<QString > ();

        for (QMap<QString, QList<Symbol> >::ConstIterator it = map.constBegin(), end = map.constEnd(); it != end; ++it) {
            set.insert(it.key());
        }

        return set;
    }

    void SyntaxTreeFactory::processLine(const QList<Symbol>& line, Tree<Symbol> tree) const throws(AnalyzeCrashExeption) {
        QStack<unsigned int> positions = QStack<unsigned int>();
        positions.push(tree.get().getType().getArgsNumber());

        for (int i = line.size() - 1; i >= 0; i--) {
            while (!tree.isRoot()) {
                if (positions.isEmpty())
                    throw AnalyzeCrashExeption();

                if (positions.top() > 0)
                    break;

                tree = tree.getSupertree();
                positions.pop();

            }

            tree[positions.top() - 1] = line[i];
            tree[positions.top() - 1].get().setId(tree.get().getId()+(char) (positions.top() + 98));
            positions.top()--;

            if (line[i].getType() == Symbol::SymbolType::AND
                    || line[i].getType() == Symbol::SymbolType::OR) {
                tree = tree[positions.top()];
                positions.push(tree.get().getType().getArgsNumber());
            }
        }
    }

    QList<Symbol> SyntaxTreeFactory::toPostfixSymbolsList(const QList<Symbol>& line) const throws(AnalyzeCrashExeption) {
        QList<Symbol> result = QList<Symbol > ();
        QStack<Symbol> stack = QStack<Symbol > ();

        for (QList<Symbol>::ConstIterator it = line.constBegin(), end = line.constEnd(); it != end; ++it) {
            if (it->getType() == Symbol::SymbolType::LITHERAL
                    || it->getType() == Symbol::SymbolType::IDENTYFIER) {
                result.append(*it);
            } else {
                if (it->getType() == Symbol::SymbolType::OPEN_BRACKET) {
                    stack.push(*it);
                } else if (it->getType() == Symbol::SymbolType::CLOSE_BRACKET) {
                    while (true) {
                        if (stack.isEmpty()) {
                            for (QList<Symbol>::ConstIterator it = line.constBegin(), end = line.constEnd(); it != end; ++it) {
                                Events::WrongBracketsNumberErrorEvent event = Events::WrongBracketsNumberErrorEvent(it->getRepresentation());
                                event.share(*broadcaster);
                            }
                            throw AnalyzeCrashExeption();
                        }

                        Symbol top = stack.pop();
                        if (top.getType() == Symbol::SymbolType::OPEN_BRACKET) break;
                        result.append(top);
                    }
                } else {
                    while (!stack.isEmpty() && it->getType().getPriority() <= stack.top().getType().getPriority()) {
                        result.append(stack.pop());
                    }
                    stack.push(*it);
                }
            }
        }

        while (!stack.isEmpty()) {
            Symbol top = stack.pop();
            if (top.getType() == Symbol::SymbolType::OPEN_BRACKET
                    || top.getType() == Symbol::SymbolType::CLOSE_BRACKET) {
                for (QList<Symbol>::ConstIterator it = line.constBegin(), end = line.constEnd(); it != end; ++it) {
                    Events::WrongBracketsNumberErrorEvent event = Events::WrongBracketsNumberErrorEvent(it->getRepresentation());
                    event.share(*broadcaster);
                }
                throw AnalyzeCrashExeption();
            }
            result.append(top);
        }

        return result;
    }

}

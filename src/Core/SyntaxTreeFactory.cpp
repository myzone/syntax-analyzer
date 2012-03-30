#include <QRegExp>
#include <QLocale>
#include <QString>

#include "../Events/Event.h"
#include "../Core/Analyzer.h"
#include "../Core/SyntaxTreeFactory.h"
#include "../Core/Preprocessor.h"

namespace Core {

    SyntaxTreeFactory::SyntaxTreeFactory(Events::EventBroadcaster* broacaster) : broadcaster(broacaster) { }

    Tree<Symbol> SyntaxTreeFactory::createTree(const QString& text) const throws(AnalyzeCrashExeption) {
        const QString MAIN_SYMBOL = "main";

        QMap<QString, QString> linesMap = createLinesMap(text);

        QSet<QString> notUsedSymbolsSet = createLinesSet(linesMap);
        QSet<QString> notDefinedSymbolsSet = QSet<QString > ();

        Tree<Symbol> tree = Tree<Symbol > ();
        QMap<QString, QString>::Iterator it = linesMap.find(MAIN_SYMBOL);
        QString value = *it;
        linesMap.erase(it);
        notUsedSymbolsSet.erase(notUsedSymbolsSet.find(MAIN_SYMBOL));

        tree = Symbol(MAIN_SYMBOL, Symbol::SymbolType::IDENTYFIER);
        tree.get().setId("a");

        processLine(value, tree);
        TreeProcessor processor = TreeProcessor();

        bool ok = true;
        while (true) {
            tree.traverse(processor);

            if (processor.isEnd())
                break;

            QList<Tree<Symbol> > nodes = processor.getNodes();

            for (QList<Tree<Symbol> >::Iterator it = nodes.begin(); it != nodes.end(); ++it) {
                QString current = (*it).get().getRepresentation();

                if (!linesMap.contains(current)) {
                    std::cout << "not def: " << current.toStdString() << "\n";
                    notDefinedSymbolsSet.insert(current);

                    ok = false;
                    continue;
                }

                notUsedSymbolsSet.erase(notUsedSymbolsSet.find(current));
                processLine(linesMap.value(current), *it);
            }

        }

        for (QSet<QString>::ConstIterator it = notDefinedSymbolsSet.begin(); it != notDefinedSymbolsSet.end(); ++it) {
            Events::SymbolIsNotDefinedErrorEvent event = Events::SymbolIsNotDefinedErrorEvent(*it);
            event.share(*broadcaster);
        }

        for (QSet<QString>::ConstIterator it = notUsedSymbolsSet.begin(); it != notUsedSymbolsSet.end(); ++it) {
            Events::SymbolIsNotUsedErrorEvent event = Events::SymbolIsNotUsedErrorEvent(*it);
            event.share(*broadcaster);
        }

        if (!ok) throw AnalyzeCrashExeption();

        return tree;
    }

    QMap<QString, QString> SyntaxTreeFactory::createLinesMap(const QString& text) const {
        const QRegExp TEXT_SEPARATOR = QRegExp("\\s*;\\s*(\n|\\s)+");
        const QRegExp LINE_SEPARATOR = QRegExp("\\s*->\\s*");

        QMap<QString, QString> map = QMap<QString, QString > ();

        QStringList lines = text.split(TEXT_SEPARATOR, QString::SkipEmptyParts);

        for (QStringList::ConstIterator it = lines.begin(); it != lines.end(); ++it) {
            QStringList temp = (*it).split(LINE_SEPARATOR, QString::SkipEmptyParts);

            if (temp.count() > 2) {
                for (int i = 2; i < temp.count(); i++) {
                    temp[1] += temp[i];
                }
            }

            map.insert(temp[0], temp[1]);
        }

        return map;
    }

    QSet<QString> SyntaxTreeFactory::createLinesSet(const QMap<QString, QString>& map) const {
        QSet<QString> set = QSet<QString > ();

        for (QMap<QString, QString>::ConstIterator it = map.begin(); it != map.end(); ++it) {
            set.insert(it.key());
        }

        return set;
    }

    void SyntaxTreeFactory::processLine(const QString& line, Tree<Symbol> tree) const throws(AnalyzeCrashExeption) {
        QList<Symbol> postfixSymbolsList = toPostfixSymbolsList(line);

        QStack<unsigned int> positions = QStack<unsigned int>();
        positions.push(tree.get().getType().getArgsNumber());
        for (int i = postfixSymbolsList.size() - 1; i >= 0; i--) {
            while (!tree.isRoot() && positions.top() <= 0) {
                tree = tree.getSupertree();
                positions.pop();
            }

            tree[positions.top() - 1] = postfixSymbolsList[i];
            tree[positions.top() - 1].get().setId(tree.get().getId()+(char) (positions.top() + 98));
            positions.top()--;

            if (postfixSymbolsList[i].getType() == Symbol::SymbolType::AND
                    || postfixSymbolsList[i].getType() == Symbol::SymbolType::OR) {
                tree = tree[positions.top()];
                positions.push(tree.get().getType().getArgsNumber());
            }

        }
    }

    QList<Symbol> SyntaxTreeFactory::toPostfixSymbolsList(const QString& line) const throws(AnalyzeCrashExeption) {
        QList<Symbol> result = QList<Symbol > ();
        QStack<Symbol> stack = QStack<Symbol > ();

        SymbolFactory symbolFactory = SymbolFactory(line);

        while (symbolFactory.isNextSymbol()) {
            Symbol current;
            try {
                current = symbolFactory.getNextSymbol();
            } catch (AnalyzeCrashExeption exeption) {
                Events::LitheralIsNotClosedErrorEvent event = Events::LitheralIsNotClosedErrorEvent(exeption.getMessage());
                event.share(*broadcaster);

                throw;
            }
            if (current.getType() == Symbol::SymbolType::LITHERAL
                    || current.getType() == Symbol::SymbolType::IDENTYFIER) {
                result.append(current);
            } else {
                if (current.getType() == Symbol::SymbolType::OPEN_BRACKET) {
                    stack.push(current);
                } else if (current.getType() == Symbol::SymbolType::CLOSE_BRACKET) {
                    while (true) {
                        if (stack.empty()) {
                            Events::WrongBracketsNumberErrorEvent event = Events::WrongBracketsNumberErrorEvent();
                            event.share(*broadcaster);
                            throw AnalyzeCrashExeption(line);
                        }

                        Symbol top = stack.pop();
                        if (top.getType() == Symbol::SymbolType::OPEN_BRACKET) break;
                        result.append(top);
                    }
                } else {
                    while (!stack.empty() && current.getType().getPriority() <= stack.top().getType().getPriority()) {
                        result.append(stack.pop());
                    }
                    stack.push(current);
                }
            }
        }


        while (!stack.empty()) {
            Symbol top = stack.pop();
            if (top.getType() == Symbol::SymbolType::OPEN_BRACKET
                    || top.getType() == Symbol::SymbolType::CLOSE_BRACKET) {
                Events::WrongBracketsNumberErrorEvent event = Events::WrongBracketsNumberErrorEvent();
                event.share(*broadcaster);
                throw AnalyzeCrashExeption(line);
            }
            result.append(top);
        }

        return result;
    }

}

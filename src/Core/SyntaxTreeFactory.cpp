#include <QRegExp>
#include <QLocale>
#include <QString>

#include "../Events/Event.h"
#include "../Core/Analyzer.h"
#include "../Core/SyntaxTreeFactory.h"
#include "Preprocessor.h"


#include <iostream>
#define cout std::cout


namespace Core {

    bool Symbol::SymbolType::AndOperation(const QList<bool>& args) {
        for (QList<bool>::ConstIterator i = args.begin(); i != args.end(); i++) {
            if (*i) {
                return false;
            }
        }

        return true;
    }

    bool Symbol::SymbolType::OrOperation(const QList<bool>& args) {
        for (QList<bool>::ConstIterator i = args.begin(); i != args.end(); i++) {
            if (*i) {
                return true;
            }
        }

        return false;
    }

    bool Symbol::SymbolType::EqualOperation(const QList<bool>& args) {
        return args[0];
    }

    bool Symbol::SymbolType::OtherOperation(const QList<bool>& args) {
        return false;
    }

    const Symbol::SymbolType Symbol::SymbolType::CLOSE_BRACKET = Symbol::SymbolType(')', &OtherOperation, 0, 0);
    const Symbol::SymbolType Symbol::SymbolType::OPEN_BRACKET = Symbol::SymbolType('(', &OtherOperation, 0, 1);
    const Symbol::SymbolType Symbol::SymbolType::OR = Symbol::SymbolType('|', &OrOperation, 2, 2);
    const Symbol::SymbolType Symbol::SymbolType::AND = Symbol::SymbolType('&', &AndOperation, 2, 3);

    const Symbol::SymbolType Symbol::SymbolType::LITHERAL = Symbol::SymbolType('\"', &OtherOperation, 0);
    const Symbol::SymbolType Symbol::SymbolType::IDENTYFIER = Symbol::SymbolType('*', &EqualOperation, 1);
    const Symbol::SymbolType Symbol::SymbolType::SPACE = Symbol::SymbolType(' ', &OtherOperation, 0);
    const Symbol::SymbolType Symbol::SymbolType::BACKSLASH = Symbol::SymbolType('\\', &OtherOperation, 0);

    Symbol::SymbolType::SymbolType() : Enum<Symbol::SymbolTypeData>() {
    }

    Symbol::SymbolType::SymbolType(const QChar& symbol, bool (*operation) (const QList<bool>& args), unsigned int argsNumber, unsigned int priority) {
        value.symbol = symbol;
        value.operation = operation;
        value.argsNumber = argsNumber;
        value.priority = priority;
    }

    Symbol::Symbol() {
    }

    Symbol::Symbol(const Symbol& that) : string(that.string), type(that.type) {
    }

    Symbol::Symbol(const QString& string, const Symbol::SymbolType& type) : string(string), type(type) {
    }

    Symbol::Symbol(const Symbol::SymbolType& type) : string(), type(type) {
        this->string += type.toChar();
    }

    const QString& Symbol::toString() const {
        return string;
    }

    const Symbol::SymbolType& Symbol::getType() const {
        return type;
    }

    SymbolFactory::SymbolFactory(const QString& line) : line(line),
    position(this->line.begin()),
    lastString(""),
    lastSymbolType(Symbol::SymbolType::OPEN_BRACKET) {
    }

    Symbol SymbolFactory::getNextSymbol() throws(AnalyzeCrashExeption, WarningExeption) {
        skipAll(Symbol::SymbolType::BACKSLASH);

        if (lastString != "") {
            Symbol result = Symbol(lastString, lastSymbolType);
            lastString = "";
            return result;
        }

        QString currentString = "";

        while (position != line.end()) {
            if (*position == Symbol::SymbolType::OPEN_BRACKET.toChar()) {
                position++;
                skipAll(Symbol::SymbolType::SPACE);

                if (lastSymbolType == Symbol::SymbolType::CLOSE_BRACKET
                        || lastSymbolType == Symbol::SymbolType::IDENTYFIER
                        || lastSymbolType == Symbol::SymbolType::LITHERAL) {
                    lastString += Symbol::SymbolType::OPEN_BRACKET.toChar();

                    lastSymbolType = Symbol::SymbolType::OPEN_BRACKET;
                    return Symbol(Symbol::SymbolType::AND);
                }

                lastSymbolType = Symbol::SymbolType::OPEN_BRACKET;
                return Symbol(Symbol::SymbolType::OPEN_BRACKET);
            } else if (*position == Symbol::SymbolType::CLOSE_BRACKET.toChar()) {
                position++;
                skipAll(Symbol::SymbolType::SPACE);

                lastSymbolType = Symbol::SymbolType::CLOSE_BRACKET;
                return Symbol(Symbol::SymbolType::CLOSE_BRACKET);
            } else if (*position == Symbol::SymbolType::AND.toChar()) {
                position++;
                skipAll(Symbol::SymbolType::SPACE);

                lastSymbolType = Symbol::SymbolType::AND;
                return Symbol(Symbol::SymbolType::AND);
            } else if (*position == Symbol::SymbolType::OR.toChar()) {
                position++;
                skipAll(Symbol::SymbolType::SPACE);

                lastSymbolType = Symbol::SymbolType::OR;
                return Symbol(Symbol::SymbolType::OR);
            } else if (*position == Symbol::SymbolType::LITHERAL.toChar()) {
                currentString += *position;
                position++;

                bool escaped = false;
                while (true) {
                    if (position == line.end()) {
                        throw AnalyzeCrashExeption(currentString);
                    }

                    currentString += *position;

                    if (*position == Symbol::SymbolType::LITHERAL.toChar() && !escaped) {
                        position++;
                        break;
                    }

                    if (escaped) {
                        escaped = false;
                    }

                    if (*position == Symbol::SymbolType::BACKSLASH.toChar() && !escaped) {
                        escaped = true;
                    }

                    position++;
                }

                skipAll(Symbol::SymbolType::SPACE);

                if (lastSymbolType == Symbol::SymbolType::CLOSE_BRACKET
                        || lastSymbolType == Symbol::SymbolType::IDENTYFIER
                        || lastSymbolType == Symbol::SymbolType::LITHERAL) {

                    lastString = currentString;
                    lastSymbolType = Symbol::SymbolType::LITHERAL;

                    return Symbol(Symbol::SymbolType::AND);
                }

                lastSymbolType = Symbol::SymbolType::LITHERAL;
                return Symbol(currentString, Symbol::SymbolType::LITHERAL);
            } else {
                while (position != line.end()
                        && *position != Symbol::SymbolType::AND.toChar()
                        && *position != Symbol::SymbolType::OR.toChar()
                        && *position != Symbol::SymbolType::OPEN_BRACKET.toChar()
                        && *position != Symbol::SymbolType::CLOSE_BRACKET.toChar()
                        && *position != Symbol::SymbolType::BACKSLASH.toChar()
                        && *position != Symbol::SymbolType::LITHERAL.toChar()
                        && *position != Symbol::SymbolType::SPACE.toChar()) {
                    currentString += *position;
                    position++;
                }

                skipAll(Symbol::SymbolType::SPACE);

                if (lastSymbolType == Symbol::SymbolType::CLOSE_BRACKET
                        || lastSymbolType == Symbol::SymbolType::IDENTYFIER
                        || lastSymbolType == Symbol::SymbolType::LITHERAL) {
                    lastString = currentString;
                    lastSymbolType = Symbol::SymbolType::IDENTYFIER;

                    return Symbol(Symbol::SymbolType::AND);
                }

                lastSymbolType = Symbol::SymbolType::IDENTYFIER;
                return Symbol(currentString, Symbol::SymbolType::IDENTYFIER);
            }
        }

        throw WarningExeption();
    }

    bool SymbolFactory::isNextSymbol() {
        return position != line.end() || !lastString.isEmpty();
    }

    void SymbolFactory::skipFirst(const Symbol::SymbolType & symbolType) {
        if (position != line.end() && *position != symbolType.toChar()) {
            position++;
        }
    }

    void SymbolFactory::skipAll(const Symbol::SymbolType & symbolType) {
        while (position != line.end()) {
            if (*position != symbolType.toChar()) {
                return;
            }
            position++;
        }
    }

    SyntaxTreeFactory::SyntaxTreeFactory(Events::EventBroadcaster* broacaster) : broadcaster(broacaster) {
    }

    Tree<Symbol> SyntaxTreeFactory::createTree(const QString& text) const throws(AnalyzeCrashExeption) {
        const QString MAIN_SYMBOL = "main";

        QMap<QString, QString> map = createLinesMap(text);

        Tree<Symbol> tree = Tree<Symbol > ();
        QMap<QString, QString>::Iterator it = map.find(MAIN_SYMBOL);
        QString value = *it;
        map.erase(it);

        tree.set(Symbol(MAIN_SYMBOL, Symbol::SymbolType::IDENTYFIER));

        processLine(value, tree);
        TreeProcessor processor = TreeProcessor(&map);

        while (true) {
            tree.walk(processor);

            if (processor.isEnd()) {
                break;
            } else {
                QMap<QString, QString>::Iterator it = map.find(processor.getSymbol());
                QString value = *it;
                map.erase(it);
                processLine(value, tree);
            }
        }

        return tree;
    }

    QMap<QString, QString> SyntaxTreeFactory::createLinesMap(const QString& text) const {
        const QRegExp TEXT_SEPARATOR = QRegExp("\\s*;(\\s|\n)*");
        const QRegExp LINE_SEPARATOR = QRegExp("\\s*->\\s*");

        QMap<QString, QString> map = QMap<QString, QString > ();

        QStringList lines = text.split(TEXT_SEPARATOR);

        for (QStringList::ConstIterator i = lines.begin(); i != lines.end(); i++) {
            QStringList temp = text.split(LINE_SEPARATOR);

            map.insert(temp[0], temp[1]);
        }

        return map;
    }

    void SyntaxTreeFactory::processLine(const QString& line, Tree<Symbol> tree) const throws(AnalyzeCrashExeption) {
        QList<Symbol> postfixSymbolsList = toPostfixSymbolsList(line);

        QStack<unsigned int> positions = QStack<unsigned int>();
        positions.push(0);
        for (int i = postfixSymbolsList.size()-1; i>=0; i--) {
            qDebug(postfixSymbolsList[i].toString().toStdString().c_str(), "\n");
            
            while (positions.top() + 1 > tree.get().getType().getArgsNumber()) {
                tree = tree.getSuperTree();
                positions.pop();
            }
            tree[positions.top()++].set(postfixSymbolsList[i]);

            if (postfixSymbolsList[i].getType() == Symbol::SymbolType::AND
                    || postfixSymbolsList[i].getType() == Symbol::SymbolType::OR) {
                tree = tree.getSubTree(positions.top() - 1);
                positions.push(0);
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

#include <QRegExp>
#include <qt4/QtCore/qlocale.h>
#include <qt4/QtCore/qstring.h>

#include "../Events/Event.h"
#include "Analyzer.h"
#include "SyntaxTreeFactory.h"


#include <iostream>
#define cout std::cout


namespace Core {

    // functional
    const Symbol::SymbolType Symbol::SymbolType::CLOSE_BRACKET = Symbol::SymbolType(')', 0);
    const Symbol::SymbolType Symbol::SymbolType::OPEN_BRACKET = Symbol::SymbolType('(', 1);
    const Symbol::SymbolType Symbol::SymbolType::OR = Symbol::SymbolType('|', 2);
    const Symbol::SymbolType Symbol::SymbolType::AND = Symbol::SymbolType('&', 3);

    // non-functional
    const Symbol::SymbolType Symbol::SymbolType::LITHERAL = Symbol::SymbolType('\"', 5);
    const Symbol::SymbolType Symbol::SymbolType::IDENTYFIER = Symbol::SymbolType('*', 5); // OTHER
    const Symbol::SymbolType Symbol::SymbolType::SPACE = Symbol::SymbolType(' ', 5);
    const Symbol::SymbolType Symbol::SymbolType::BACKSLASH = Symbol::SymbolType('\\', 5);

    Symbol::SymbolType::SymbolType() : Enum<Symbol::SymbolTypeData>() {
    }

    Symbol::SymbolType::SymbolType(const QChar& symbol, int priority) {
        value.symbol = symbol;
        value.priority = priority;
    }

    Symbol::Symbol() {
    }

    Symbol::Symbol(const QString& string, const Symbol::SymbolType& type) : string(string), type(type) {
    }

    Symbol::Symbol(const Symbol::SymbolType& type) : string(), type(type) {
        this->string += type.toChar();
    }

    const QString& Symbol::getString() const {
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

    SymbolFactory::SymbolFactory(const SymbolFactory& that) : line(that.line),
    position(that.position),
    lastString(that.lastString),
    lastSymbolType(that.lastSymbolType) {
    }

    /* 
     * ( ( a"1" | b "2") c | d )
     *            ^
     * (.(.a."1".|.b."2".).&.c.|.d.).
     * 
     * " ( a \"+\" b ) | c"
     */


    Symbol SymbolFactory::getNextSymbol() throws(AnalyzeCrashExeption, WarningExeption) {
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
                while (position != line.end() && *position != Symbol::SymbolType::SPACE.toChar()) {
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
        return position != line.end();
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

    Tree<QString> createTree(const QString& text) {
        return Tree<QString > ();
    }

    void processLine(const QString& line, Tree<QString>& tree) {

    }

    QList<QString> SyntaxTreeFactory::toSymbolList(const QString& line) throws(AnalyzeCrashExeption) {
        QList<QString> result = QList<QString > ();

        QStack<Symbol> stack = QStack<Symbol > ();

        SymbolFactory symbolFactory = SymbolFactory(line);

        while (symbolFactory.isNextSymbol()) {
            Symbol current = symbolFactory.getNextSymbol();

            if (current.getType() == Symbol::SymbolType::LITHERAL
                    || current.getType() == Symbol::SymbolType::IDENTYFIER) {
                result.append(current.getString());
            } else {
                if (current.getType() == Symbol::SymbolType::OPEN_BRACKET) {
                    stack.push(current);
                } else if (current.getType() == Symbol::SymbolType::CLOSE_BRACKET) {
                    while (true) {
                        if (stack.empty()) throw AnalyzeCrashExeption(line);
                        Symbol top = stack.pop();

                        if (top.getType() == Symbol::SymbolType::OPEN_BRACKET) break;

                        result.append(top.getString());
                    }
                } else {
                    while (!stack.empty() && current.getType().getPriority() <= stack.top().getType().getPriority()) {
                        result.append(stack.pop().getString());
                    }
                    stack.push(current);
                }
            }
        }


        while (!stack.empty()) {
            result.append(stack.pop().getString());
        }

        return result;
    }

}
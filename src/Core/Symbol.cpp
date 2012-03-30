#include "Symbol.h"

#include <iostream>

namespace Core {

    bool Symbol::SymbolType::andOperation(const QList<bool>& args) {
        for (QList<bool>::ConstIterator it = args.begin(); it != args.end(); ++it) {
            if (!*it) {
                return false;
            }
        }

        return true;
    }

    bool Symbol::SymbolType::orOperation(const QList<bool>& args) {
        for (QList<bool>::ConstIterator it = args.begin(); it != args.end(); ++it) {
            if (*it) {
                return true;
            }
        }

        return false;
    }

    bool Symbol::SymbolType::equalOperation(const QList<bool>& args) {
        return !args.isEmpty() ? args[0] : false;
    }

    bool Symbol::SymbolType::litheralOperation(const QList<bool>& args) {
        return true;
    }

    bool Symbol::SymbolType::otherOperation(const QList<bool>& args) {
        return false;
    }

    const Symbol::SymbolType Symbol::SymbolType::CLOSE_BRACKET = Symbol::SymbolType(")", &otherOperation, 0, 0);
    const Symbol::SymbolType Symbol::SymbolType::OPEN_BRACKET = Symbol::SymbolType("(", &otherOperation, 0, 1);
    const Symbol::SymbolType Symbol::SymbolType::OR = Symbol::SymbolType("|", &orOperation, 2, 2);
    const Symbol::SymbolType Symbol::SymbolType::AND = Symbol::SymbolType("&", &andOperation, 2, 3);

    const Symbol::SymbolType Symbol::SymbolType::LITHERAL = Symbol::SymbolType("\"", &litheralOperation, 0);
    const Symbol::SymbolType Symbol::SymbolType::IDENTYFIER = Symbol::SymbolType("", &equalOperation, 1);
    const Symbol::SymbolType Symbol::SymbolType::SPACE = Symbol::SymbolType(" ", &otherOperation, 0);
    const Symbol::SymbolType Symbol::SymbolType::BACKSLASH = Symbol::SymbolType("\\", &otherOperation, 0);
    const Symbol::SymbolType Symbol::SymbolType::DEFINE = Symbol::SymbolType("->", &otherOperation, 0);
    const Symbol::SymbolType Symbol::SymbolType::DEFINE_END = Symbol::SymbolType(";", &otherOperation, 0);

    Symbol::SymbolType::SymbolType() : Enum<Symbol::SymbolTypeData>() { }

    Symbol::SymbolType::SymbolType(const QString& symbol, bool (*operation) (const QList<bool>& args), unsigned int argsNumber, unsigned int priority) {
        value.symbol = symbol;
        value.operation = operation;
        value.argsNumber = argsNumber;
        value.priority = priority;
    }

    Symbol::Symbol() { }

    Symbol::Symbol(const QString& representation, const Symbol::SymbolType& type) : representation(representation), type(type), id("") { }

    Symbol::Symbol(const Symbol::SymbolType& type) : representation(), type(type) {
        this->representation += type.toString();
    }

    ///////////////////////////////////////////////////////////////////////////

    SymbolFactory::SymbolFactory(const QString& line) : line(line),
    position(this->line.begin()),
    end(this->line.end()),
    lastString(""),
    lastSymbolType(Symbol::SymbolType::OPEN_BRACKET) { }

    Symbol SymbolFactory::getNextSymbol() throws(AnalyzeCrashExeption, WarningExeption) {
        if (lastString != "") {
            Symbol result = Symbol(lastString, lastSymbolType);
            lastString = "";
            return result;
        }

        QString currentString = "";

        while (position != line.end()) {
            skipAll(Symbol::SymbolType::SPACE);

            if (whetherNextSymbol(Symbol::SymbolType::OPEN_BRACKET)) {
                position += Symbol::SymbolType::OPEN_BRACKET.toString().length();

                if (lastSymbolType == Symbol::SymbolType::CLOSE_BRACKET
                        || lastSymbolType == Symbol::SymbolType::IDENTYFIER
                        || lastSymbolType == Symbol::SymbolType::LITHERAL) {
                    lastString += Symbol::SymbolType::OPEN_BRACKET.toString();

                    lastSymbolType = Symbol::SymbolType::OPEN_BRACKET;
                    return Symbol(Symbol::SymbolType::AND);
                }

                lastSymbolType = Symbol::SymbolType::OPEN_BRACKET;
                return Symbol(Symbol::SymbolType::OPEN_BRACKET);
            } else if (whetherNextSymbol(Symbol::SymbolType::CLOSE_BRACKET)) {
                position += Symbol::SymbolType::CLOSE_BRACKET.toString().length();

                lastSymbolType = Symbol::SymbolType::CLOSE_BRACKET;
                return Symbol(Symbol::SymbolType::CLOSE_BRACKET);
            } else if (whetherNextSymbol(Symbol::SymbolType::AND)) {
                position += Symbol::SymbolType::AND.toString().length();

                lastSymbolType = Symbol::SymbolType::AND;
                return Symbol(Symbol::SymbolType::AND);
            } else if (whetherNextSymbol(Symbol::SymbolType::OR)) {
                position += Symbol::SymbolType::OR.toString().length();

                lastSymbolType = Symbol::SymbolType::OR;
                return Symbol(Symbol::SymbolType::OR);
            } else if (whetherNextSymbol(Symbol::SymbolType::DEFINE)) {
                position += Symbol::SymbolType::DEFINE.toString().length();

                lastSymbolType = Symbol::SymbolType::DEFINE;
                return Symbol(Symbol::SymbolType::DEFINE);
            } else if (whetherNextSymbol(Symbol::SymbolType::DEFINE_END)) {
                position += Symbol::SymbolType::DEFINE_END.toString().length();

                lastSymbolType = Symbol::SymbolType::DEFINE_END;
                return Symbol(Symbol::SymbolType::DEFINE_END);
            } else if (whetherNextSymbol(Symbol::SymbolType::LITHERAL)) {
                currentString += *position;
                ++position;

                bool escaped = false;
                while (true) {
                    if (position == line.end()) {
                        throw AnalyzeCrashExeption(currentString);
                    }

                    currentString += *position;

                    if (whetherNextSymbol(Symbol::SymbolType::LITHERAL) && !escaped) {
                        ++position;
                        break;
                    }

                    if (escaped) {
                        escaped = false;
                    }

                    if (whetherNextSymbol(Symbol::SymbolType::BACKSLASH) && !escaped) {
                        escaped = true;
                    }

                    ++position;
                }


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
                        && !whetherNextSymbol(Symbol::SymbolType::AND)
                        && !whetherNextSymbol(Symbol::SymbolType::OR)
                        && !whetherNextSymbol(Symbol::SymbolType::OPEN_BRACKET)
                        && !whetherNextSymbol(Symbol::SymbolType::CLOSE_BRACKET)
                        && !whetherNextSymbol(Symbol::SymbolType::BACKSLASH)
                        && !whetherNextSymbol(Symbol::SymbolType::LITHERAL)
                        && !whetherNextSymbol(Symbol::SymbolType::DEFINE)
                        && !whetherNextSymbol(Symbol::SymbolType::DEFINE_END)
                        && !whetherNextSymbol(Symbol::SymbolType::SPACE)) {
                    currentString += *position;
                    ++position;
                }

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

    void SymbolFactory::skipFirst(const Symbol::SymbolType& symbolType) {
        if (position != line.end() && whetherNextSymbol(symbolType)) {
            position += symbolType.toString().length();
        }
    }

    void SymbolFactory::skipAll(const Symbol::SymbolType& symbolType) {
        while (position != line.end()) {
            if (!whetherNextSymbol(symbolType)) {
                return;
            }
            position += symbolType.toString().length();
        }
    }

    bool SymbolFactory::whetherNextSymbol(const Symbol::SymbolType& symbolType) {
        QString::ConstIterator temp = position;

        QString symbolTypeRepresentation = symbolType.toString();
        for (QString::ConstIterator it = symbolTypeRepresentation.begin(); it != symbolTypeRepresentation.end(); ++it) {
            if (*temp != *it || temp == end)
                return false;

            ++temp;
        }

        return true;
    }

}
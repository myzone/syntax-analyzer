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
    const Symbol::SymbolType Symbol::SymbolType::LINE_END = Symbol::SymbolType("\n", &otherOperation, 0);

    const Symbol::SymbolType Symbol::SymbolType::BACKSLASH = Symbol::SymbolType("\\", &otherOperation, 0);
    const Symbol::SymbolType Symbol::SymbolType::DEFINE = Symbol::SymbolType("->", &otherOperation, 0);
    const Symbol::SymbolType Symbol::SymbolType::DEFINE_END = Symbol::SymbolType(";", &otherOperation, 0);
    const Symbol::SymbolType Symbol::SymbolType::SINGLE_LINE_COMMENT_BEGIN = Symbol::SymbolType("//", &otherOperation, 0);
    const Symbol::SymbolType Symbol::SymbolType::MULTI_LINE_COMMENT_BEGIN = Symbol::SymbolType("/*", &otherOperation, 0);
    const Symbol::SymbolType Symbol::SymbolType::MULTI_LINE_COMMENT_END = Symbol::SymbolType("*/", &otherOperation, 0);

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

    SymbolFactory::SymbolFactory(const QString& source) : source(source),
    position(this->source.begin()),
    end(this->source.end()),
    lastString(""),
    lastSymbolType(Symbol::SymbolType::OPEN_BRACKET) { }

    Symbol SymbolFactory::getNextSymbol() throws(AnalyzeCrashExeption, WarningExeption) {
        if (lastString != "") {
            Symbol result = Symbol(lastString, lastSymbolType);
            lastString = "";
            return result;
        }

        QString currentString = "";

        while (position != source.end()) {
            skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);

            if (whetherNextSymbol(Symbol::SymbolType::OPEN_BRACKET)) {
                position += Symbol::SymbolType::OPEN_BRACKET.toString().length();

                if (lastSymbolType == Symbol::SymbolType::CLOSE_BRACKET
                        || lastSymbolType == Symbol::SymbolType::IDENTYFIER
                        || lastSymbolType == Symbol::SymbolType::LITHERAL) {
                    lastString += Symbol::SymbolType::OPEN_BRACKET.toString();

                    lastSymbolType = Symbol::SymbolType::OPEN_BRACKET;
                    skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                    return Symbol(Symbol::SymbolType::AND);
                }

                lastSymbolType = Symbol::SymbolType::OPEN_BRACKET;
                skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                return Symbol(Symbol::SymbolType::OPEN_BRACKET);
            } else if (whetherNextSymbol(Symbol::SymbolType::CLOSE_BRACKET)) {
                position += Symbol::SymbolType::CLOSE_BRACKET.toString().length();

                lastSymbolType = Symbol::SymbolType::CLOSE_BRACKET;
                skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                return Symbol(Symbol::SymbolType::CLOSE_BRACKET);
            } else if (whetherNextSymbol(Symbol::SymbolType::AND)) {
                position += Symbol::SymbolType::AND.toString().length();

                lastSymbolType = Symbol::SymbolType::AND;
                skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                return Symbol(Symbol::SymbolType::AND);
            } else if (whetherNextSymbol(Symbol::SymbolType::OR)) {
                position += Symbol::SymbolType::OR.toString().length();

                lastSymbolType = Symbol::SymbolType::OR;
                skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                return Symbol(Symbol::SymbolType::OR);
            } else if (whetherNextSymbol(Symbol::SymbolType::DEFINE)) {
                position += Symbol::SymbolType::DEFINE.toString().length();

                lastSymbolType = Symbol::SymbolType::DEFINE;
                skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                return Symbol(Symbol::SymbolType::DEFINE);
            } else if (whetherNextSymbol(Symbol::SymbolType::DEFINE_END)) {
                position += Symbol::SymbolType::DEFINE_END.toString().length();

                lastSymbolType = Symbol::SymbolType::DEFINE_END;
                skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                return Symbol(Symbol::SymbolType::DEFINE_END);
            } else if (whetherNextSymbol(Symbol::SymbolType::SINGLE_LINE_COMMENT_BEGIN)) {
                position += Symbol::SymbolType::SINGLE_LINE_COMMENT_BEGIN.toString().length();

                while (!whetherNextSymbol(Symbol::SymbolType::LINE_END)) {
                    position++;
                }

                lastSymbolType = Symbol::SymbolType::SINGLE_LINE_COMMENT_BEGIN;
                skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                return Symbol(Symbol::SymbolType::SINGLE_LINE_COMMENT_BEGIN);
            } else if (whetherNextSymbol(Symbol::SymbolType::MULTI_LINE_COMMENT_BEGIN)) {
                position += Symbol::SymbolType::MULTI_LINE_COMMENT_BEGIN.toString().length();

                while (whetherNextSymbol(Symbol::SymbolType::MULTI_LINE_COMMENT_END)) {
                    position++;
                }

                lastSymbolType = Symbol::SymbolType::MULTI_LINE_COMMENT_BEGIN;
                skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                return Symbol(Symbol::SymbolType::MULTI_LINE_COMMENT_BEGIN);
            } else if (whetherNextSymbol(Symbol::SymbolType::MULTI_LINE_COMMENT_END)) {
                position += Symbol::SymbolType::MULTI_LINE_COMMENT_END.toString().length();

                lastSymbolType = Symbol::SymbolType::MULTI_LINE_COMMENT_END;
                skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                return Symbol(Symbol::SymbolType::MULTI_LINE_COMMENT_END);
            } else if (whetherNextSymbol(Symbol::SymbolType::LITHERAL)) {
                currentString += *position;
                ++position;

                bool escaped = false;
                while (true) {
                    if (position == source.end()) {
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

                    skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                    return Symbol(Symbol::SymbolType::AND);
                }

                lastSymbolType = Symbol::SymbolType::LITHERAL;
                skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                return Symbol(currentString, Symbol::SymbolType::LITHERAL);
            } else {
                while (position != source.end()
                        && !whetherNextSymbol(Symbol::SymbolType::AND)
                        && !whetherNextSymbol(Symbol::SymbolType::OR)
                        && !whetherNextSymbol(Symbol::SymbolType::OPEN_BRACKET)
                        && !whetherNextSymbol(Symbol::SymbolType::CLOSE_BRACKET)
                        && !whetherNextSymbol(Symbol::SymbolType::BACKSLASH)
                        && !whetherNextSymbol(Symbol::SymbolType::LITHERAL)
                        && !whetherNextSymbol(Symbol::SymbolType::DEFINE)
                        && !whetherNextSymbol(Symbol::SymbolType::DEFINE_END)
                        && !whetherNextSymbol(Symbol::SymbolType::SINGLE_LINE_COMMENT_BEGIN)
                        && !whetherNextSymbol(Symbol::SymbolType::MULTI_LINE_COMMENT_BEGIN)
                        && !whetherNextSymbol(Symbol::SymbolType::MULTI_LINE_COMMENT_END)
                        && !whetherNextSymbol(Symbol::SymbolType::SPACE)
                        && !whetherNextSymbol(Symbol::SymbolType::LINE_END)) {
                    currentString += *position;
                    ++position;
                }

                if (lastSymbolType == Symbol::SymbolType::CLOSE_BRACKET
                        || lastSymbolType == Symbol::SymbolType::IDENTYFIER
                        || lastSymbolType == Symbol::SymbolType::LITHERAL) {
                    lastString = currentString;
                    lastSymbolType = Symbol::SymbolType::IDENTYFIER;

                    skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                    return Symbol(Symbol::SymbolType::AND);
                }

                lastSymbolType = Symbol::SymbolType::IDENTYFIER;
                skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                return Symbol(currentString, Symbol::SymbolType::IDENTYFIER);
            }
        }

        throw WarningExeption();
    }

    bool SymbolFactory::isNextSymbol() {
        return position != source.end() || !lastString.isEmpty();
    }

    void SymbolFactory::skipFirst(const Symbol::SymbolType& symbolType) {
        if (position != source.end() && whetherNextSymbol(symbolType)) {
            position += symbolType.toString().length();
        }
    }

    void SymbolFactory::skipAll(const Symbol::SymbolType& symbolType) {
        while (position != source.end()) {
            if (!whetherNextSymbol(symbolType)) {
                return;
            }
            position += symbolType.toString().length();
        }
    }

    void SymbolFactory::skipAll(const Symbol::SymbolType& symbolTypeA, const Symbol::SymbolType& symbolTypeB) {
        while (position != source.end()) {
            bool end = true;

            if (end && whetherNextSymbol(symbolTypeA)) {
                position += symbolTypeA.toString().length();
                end = false;
            }

            if (end && whetherNextSymbol(symbolTypeB)) {
                position += symbolTypeA.toString().length();
                end = false;
            }

            if (end) return;
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
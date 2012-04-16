#include "Symbol.h"
#include "src/Events/Event.h"

#include <iostream>

namespace Core {

    bool Symbol::SymbolType::andOperation(const QList<bool>& args) {
        for (QList<bool>::ConstIterator it = args.constBegin(), end = args.constEnd(); it != end; ++it) {
            if (!*it) {
                return false;
            }
        }

        return true;
    }

    bool Symbol::SymbolType::orOperation(const QList<bool>& args) {
        for (QList<bool>::ConstIterator it = args.constBegin(), end = args.constEnd(); it != end; ++it) {
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

    SymbolFactory::SymbolFactory(const QString& source) : source(source),
    sourcePosition(this->source.constBegin()),
    sourceEnd(this->source.constEnd()),
    lastString(""),
    lastSymbolType(Symbol::SymbolType::OPEN_BRACKET) {
        skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
    }

    Symbol SymbolFactory::getNextSymbol() throws(AnalyzeCrashExeption, WarningExeption) {
        if (lastString != "") {
            Symbol result = Symbol(lastString, lastSymbolType);
            lastString = "";
            return result;
        }

        QString currentString = "";

        while (sourcePosition != sourceEnd) {
            if (whetherNextSymbol(Symbol::SymbolType::OPEN_BRACKET)) {
                sourcePosition += Symbol::SymbolType::OPEN_BRACKET.toString().length();

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
                sourcePosition += Symbol::SymbolType::CLOSE_BRACKET.toString().length();

                lastSymbolType = Symbol::SymbolType::CLOSE_BRACKET;
                skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                return Symbol(Symbol::SymbolType::CLOSE_BRACKET);
            } else if (whetherNextSymbol(Symbol::SymbolType::AND)) {
                sourcePosition += Symbol::SymbolType::AND.toString().length();

                lastSymbolType = Symbol::SymbolType::AND;
                skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                return Symbol(Symbol::SymbolType::AND);
            } else if (whetherNextSymbol(Symbol::SymbolType::OR)) {
                sourcePosition += Symbol::SymbolType::OR.toString().length();

                lastSymbolType = Symbol::SymbolType::OR;
                skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                return Symbol(Symbol::SymbolType::OR);
            } else if (whetherNextSymbol(Symbol::SymbolType::DEFINE)) {
                sourcePosition += Symbol::SymbolType::DEFINE.toString().length();

                lastSymbolType = Symbol::SymbolType::DEFINE;
                skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                return Symbol(Symbol::SymbolType::DEFINE);
            } else if (whetherNextSymbol(Symbol::SymbolType::DEFINE_END)) {
                sourcePosition += Symbol::SymbolType::DEFINE_END.toString().length();

                lastSymbolType = Symbol::SymbolType::DEFINE_END;
                skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                return Symbol(Symbol::SymbolType::DEFINE_END);
            } else if (whetherNextSymbol(Symbol::SymbolType::SINGLE_LINE_COMMENT_BEGIN)) {
                sourcePosition += Symbol::SymbolType::SINGLE_LINE_COMMENT_BEGIN.toString().length();

                while (!whetherNextSymbol(Symbol::SymbolType::LINE_END)) {
                    if (sourcePosition == sourceEnd)
                        break;

                    ++sourcePosition;
                }

                skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                return Symbol(Symbol::SymbolType::SINGLE_LINE_COMMENT_BEGIN);
            } else if (whetherNextSymbol(Symbol::SymbolType::MULTI_LINE_COMMENT_BEGIN)) {
                currentString += Symbol::SymbolType::MULTI_LINE_COMMENT_BEGIN.toString();
                sourcePosition += Symbol::SymbolType::MULTI_LINE_COMMENT_BEGIN.toString().length();

                while (!whetherNextSymbol(Symbol::SymbolType::MULTI_LINE_COMMENT_END)) {
                    if (sourcePosition == sourceEnd)
                        throw AnalyzeCrashExeption(currentString);

                    currentString += *sourcePosition;
                    ++sourcePosition;
                }

                skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                return Symbol(Symbol::SymbolType::MULTI_LINE_COMMENT_BEGIN);
            } else if (whetherNextSymbol(Symbol::SymbolType::MULTI_LINE_COMMENT_END)) {
                sourcePosition += Symbol::SymbolType::MULTI_LINE_COMMENT_END.toString().length();

                skipAll(Symbol::SymbolType::SPACE, Symbol::SymbolType::LINE_END);
                return Symbol(Symbol::SymbolType::MULTI_LINE_COMMENT_END);
            } else if (whetherNextSymbol(Symbol::SymbolType::LITHERAL)) {
                currentString += *sourcePosition;
                ++sourcePosition;

                bool escaped = false;
                while (true) {
                    if (sourcePosition == sourceEnd) {
                        throw AnalyzeCrashExeption(currentString);
                    }

                    currentString += *sourcePosition;

                    if (whetherNextSymbol(Symbol::SymbolType::LITHERAL) && !escaped) {
                        ++sourcePosition;
                        break;
                    }

                    if (escaped) {
                        escaped = false;
                    }

                    if (whetherNextSymbol(Symbol::SymbolType::BACKSLASH) && !escaped) {
                        escaped = true;
                    }

                    ++sourcePosition;
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
                while (sourcePosition != sourceEnd
                        && !whetherNextSymbol(Symbol::SymbolType::AND)
                        && !whetherNextSymbol(Symbol::SymbolType::OR)
                        && !whetherNextSymbol(Symbol::SymbolType::OPEN_BRACKET)
                        && !whetherNextSymbol(Symbol::SymbolType::CLOSE_BRACKET)
                        && !whetherNextSymbol(Symbol::SymbolType::LITHERAL)
                        && !whetherNextSymbol(Symbol::SymbolType::DEFINE)
                        && !whetherNextSymbol(Symbol::SymbolType::DEFINE_END)
                        && !whetherNextSymbol(Symbol::SymbolType::SINGLE_LINE_COMMENT_BEGIN)
                        && !whetherNextSymbol(Symbol::SymbolType::MULTI_LINE_COMMENT_BEGIN)
                        && !whetherNextSymbol(Symbol::SymbolType::MULTI_LINE_COMMENT_END)
                        && !whetherNextSymbol(Symbol::SymbolType::SPACE)
                        && !whetherNextSymbol(Symbol::SymbolType::LINE_END)) {
                    currentString += *sourcePosition;
                    ++sourcePosition;
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

    bool SymbolFactory::isNextSymbol() const {
        return sourcePosition != sourceEnd || !lastString.isEmpty();
    }

    void SymbolFactory::skipFirst(const Symbol::SymbolType& symbolType) {
        if (sourcePosition != sourceEnd && whetherNextSymbol(symbolType)) {
            sourcePosition += symbolType.toString().length();
        }
    }

    void SymbolFactory::skipAll(const Symbol::SymbolType& symbolType) {
        while (sourcePosition != sourceEnd) {
            if (!whetherNextSymbol(symbolType)) {
                return;
            }

            sourcePosition += symbolType.toString().length();
        }
    }

    void SymbolFactory::skipAll(const Symbol::SymbolType& symbolTypeA, const Symbol::SymbolType& symbolTypeB) {
        while (sourcePosition != sourceEnd) {
            bool end = true;

            if (end && whetherNextSymbol(symbolTypeA)) {
                sourcePosition += symbolTypeA.toString().length();
                end = false;
            }

            if (end && whetherNextSymbol(symbolTypeB)) {
                sourcePosition += symbolTypeB.toString().length();
                end = false;
            }

            if (end) return;
        }
    }

    void SymbolFactory::skipAll(const Symbol::SymbolType& symbolTypeA, const Symbol::SymbolType& symbolTypeB, const Symbol::SymbolType& symbolTypeC) {
        while (sourcePosition != source.end()) {
            bool end = true;

            if (end && whetherNextSymbol(symbolTypeA)) {
                sourcePosition += symbolTypeA.toString().length();
                end = false;
            }

            if (end && whetherNextSymbol(symbolTypeB)) {
                sourcePosition += symbolTypeB.toString().length();
                end = false;
            }

            if (end && whetherNextSymbol(symbolTypeC)) {
                sourcePosition += symbolTypeC.toString().length();
                end = false;
            }

            if (end) return;
        }
    }

    bool SymbolFactory::whetherNextSymbol(const Symbol::SymbolType& symbolType) const {
        QString::ConstIterator temp = sourcePosition;

        QString symbolTypeRepresentation = symbolType.toString();
        for (QString::ConstIterator it = symbolTypeRepresentation.constBegin(), end = symbolTypeRepresentation.constEnd(); it != end; ++it) {
            if (*temp != *it || temp == sourceEnd)
                return false;

            ++temp;
        }

        return true;
    }

    BufferedFilteredSymbolFactory::BufferedFilteredSymbolFactory(const QString& text) : SymbolFactory(text), analyzeCrashHappened("") {
        try {
            while (SymbolFactory::isNextSymbol()) {
                Symbol symbol;

                while (true) {
                    symbol = SymbolFactory::getNextSymbol();

                    if (symbol.getType() != Symbol::SymbolType::SINGLE_LINE_COMMENT_BEGIN
                            && symbol.getType() != Symbol::SymbolType::MULTI_LINE_COMMENT_BEGIN
                            && symbol.getType() != Symbol::SymbolType::MULTI_LINE_COMMENT_END)
                        break;

                }

                symbols.append(symbol);
            }

            currentSymbol = symbols.constBegin();
            symbolsEnd = symbols.constEnd();
        } catch (AnalyzeCrashExeption exeption) {
            analyzeCrashHappened = exeption.getMessage();
        } catch (WarningExeption) {
            // Unbelievable ;)
        }
    }

    Symbol BufferedFilteredSymbolFactory::getNextSymbol() throws(AnalyzeCrashExeption, WarningExeption) {
        if (!analyzeCrashHappened.isEmpty()) {
            QString temp = analyzeCrashHappened;
            analyzeCrashHappened = "";
            throw AnalyzeCrashExeption(temp);
        }

        if (currentSymbol == symbolsEnd)
            throw WarningExeption();

        return *(currentSymbol++);
    }

    bool BufferedFilteredSymbolFactory::isNextSymbol() const {
        return currentSymbol != symbolsEnd;
    }
}
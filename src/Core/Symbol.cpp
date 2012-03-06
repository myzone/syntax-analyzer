#include "Symbol.h"

namespace Core {

    bool Symbol::SymbolType::andOperation(const QList<bool>& args) {
        for (QList<bool>::ConstIterator it = args.begin(); it != args.end(); ++it) {
            if (*it) {
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
        return args[0];
    }

    bool Symbol::SymbolType::litheralOperation(const QList<bool>& args) {
        return true;
    }

    bool Symbol::SymbolType::otherOperation(const QList<bool>& args) {
        return false;
    }

    const Symbol::SymbolType Symbol::SymbolType::CLOSE_BRACKET = Symbol::SymbolType(')', &otherOperation, 0, 0);
    const Symbol::SymbolType Symbol::SymbolType::OPEN_BRACKET = Symbol::SymbolType('(', &otherOperation, 0, 1);
    const Symbol::SymbolType Symbol::SymbolType::OR = Symbol::SymbolType('|', &orOperation, 2, 2);
    const Symbol::SymbolType Symbol::SymbolType::AND = Symbol::SymbolType('&', &andOperation, 2, 3);

    const Symbol::SymbolType Symbol::SymbolType::LITHERAL = Symbol::SymbolType('\"', &litheralOperation, 0);
    const Symbol::SymbolType Symbol::SymbolType::IDENTYFIER = Symbol::SymbolType('*', &equalOperation, 1);
    const Symbol::SymbolType Symbol::SymbolType::SPACE = Symbol::SymbolType(' ', &otherOperation, 0);
    const Symbol::SymbolType Symbol::SymbolType::BACKSLASH = Symbol::SymbolType('\\', &otherOperation, 0);

    Symbol::SymbolType::SymbolType() : Enum<Symbol::SymbolTypeData>() { }

    Symbol::SymbolType::SymbolType(const QChar& symbol, bool (*operation) (const QList<bool>& args), unsigned int argsNumber, unsigned int priority) {
        value.symbol = symbol;
        value.operation = operation;
        value.argsNumber = argsNumber;
        value.priority = priority;
    }

    Symbol::Symbol() { }

    Symbol::Symbol(const QString& representation, const Symbol::SymbolType& type) : representation(representation), type(type), id("") { }

    Symbol::Symbol(const Symbol::SymbolType& type) : representation(), type(type) {
        this->representation += type.toChar();
    }

    ///////////////////////////////////////////////////////////////////////////

    SymbolFactory::SymbolFactory(const QString& line) : line(line),
    position(this->line.begin()),
    end(this->line.end()),
    lastString(""),
    lastSymbolType(Symbol::SymbolType::OPEN_BRACKET) { }

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
                ++position;
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
                ++position;
                skipAll(Symbol::SymbolType::SPACE);

                lastSymbolType = Symbol::SymbolType::CLOSE_BRACKET;
                return Symbol(Symbol::SymbolType::CLOSE_BRACKET);
            } else if (*position == Symbol::SymbolType::AND.toChar()) {
                ++position;
                skipAll(Symbol::SymbolType::SPACE);

                lastSymbolType = Symbol::SymbolType::AND;
                return Symbol(Symbol::SymbolType::AND);
            } else if (*position == Symbol::SymbolType::OR.toChar()) {
                ++position;
                skipAll(Symbol::SymbolType::SPACE);

                lastSymbolType = Symbol::SymbolType::OR;
                return Symbol(Symbol::SymbolType::OR);
            } else if (*position == Symbol::SymbolType::LITHERAL.toChar()) {
                currentString += *position;
                ++position;

                bool escaped = false;
                while (true) {
                    if (position == line.end()) {
                        throw AnalyzeCrashExeption(currentString);
                    }

                    currentString += *position;

                    if (*position == Symbol::SymbolType::LITHERAL.toChar() && !escaped) {
                        ++position;
                        break;
                    }

                    if (escaped) {
                        escaped = false;
                    }

                    if (*position == Symbol::SymbolType::BACKSLASH.toChar() && !escaped) {
                        escaped = true;
                    }

                    ++position;
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
                    ++position;
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

}
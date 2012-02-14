#include <QRegExp>

#include "../Events/Event.h"
#include "Analyzer.h"
#include "SyntaxTreeFactory.h"

namespace Core {

    const Symbol::SymbolType Symbol::SymbolType::CLOSE_BRACKET = Symbol::SymbolType(")", 0);
    const Symbol::SymbolType Symbol::SymbolType::OPEN_BRACKET = Symbol::SymbolType("(", 1);
    const Symbol::SymbolType Symbol::SymbolType::OR = Symbol::SymbolType("|", 2);
    const Symbol::SymbolType Symbol::SymbolType::AND = Symbol::SymbolType("&", 3);
    const Symbol::SymbolType Symbol::SymbolType::LITHERAL = Symbol::SymbolType("\"", 5);
    const Symbol::SymbolType Symbol::SymbolType::OTHER = Symbol::SymbolType("", 5);

    Symbol::SymbolType::SymbolType() : Enum<Symbol::SymbolTypeData>() {
    }

    Symbol::SymbolType::SymbolType(const QString string, int priority) {
        value.string = string;
        value.priority = priority;
    }

    Symbol::SymbolType::SymbolType(const Symbol::SymbolType& that) : Enum(that) {
    }

    Symbol::Symbol(const QString& string, const Symbol::SymbolType& type) : string(string), type(type) {
    }

    Symbol::Symbol(const Symbol& that) : string(that.string), type(type) {
    }

    const QString& Symbol::getString() const {
        return string;
    }

    const Symbol::SymbolType& Symbol::getType() const {
        return type;
    }

    SymbolFactory::SymbolFactory(const QString& line) : line(line),
    position(line.begin()),
    lastString(""),
    lastSymbolType(Symbol::SymbolType::AND) {
    }

    SymbolFactory::SymbolFactory(const SymbolFactory& that) : line(that.line),
    position(that.position),
    lastString(that.lastString),
    lastSymbolType(that.lastSymbolType) {
    }

    Symbol SymbolFactory::getNextSymbol() throws(AnalyzeCrashExeption, WarningExeption) {
        if (lastString != "") {
            QString temp = lastString;
            lastString = "";

            return Symbol(temp, lastSymbolType);
        }

        QString symbolString = "";
        while (position != line.end()) {
            symbolString += *position;
            position++;

            if (symbolString == Symbol::SymbolType::OPEN_BRACKET.getString()) {
                lastSymbolType = Symbol::SymbolType::OPEN_BRACKET;

                return Symbol(symbolString, Symbol::SymbolType::OPEN_BRACKET);
            }

            if (symbolString == Symbol::SymbolType::CLOSE_BRACKET.getString()) {
                lastSymbolType = Symbol::SymbolType::CLOSE_BRACKET;

                return Symbol(symbolString, Symbol::SymbolType::CLOSE_BRACKET);
            }

            if (symbolString == Symbol::SymbolType::OR.getString()) {
                lastSymbolType = Symbol::SymbolType::OR;

                return Symbol(symbolString, Symbol::SymbolType::OR);
            }

            if (symbolString == Symbol::SymbolType::AND.getString()) {
                lastSymbolType = Symbol::SymbolType::AND;

                return Symbol(Symbol::SymbolType::AND.getString(), Symbol::SymbolType::AND);
            }

            if (symbolString == Symbol::SymbolType::LITHERAL.getString()) {
                while (true) {
                    symbolString += *position;

                    if (QString(*position) == Symbol::SymbolType::LITHERAL.getString()) {
                        position++;
                        break;
                    }

                    if (position == line.end()) {
                        throw AnalyzeCrashExeption(line);
                    }

                    position++;
                }

                if (lastSymbolType == Symbol::SymbolType::OTHER) {
                    lastString = symbolString;
                    lastSymbolType = Symbol::SymbolType::LITHERAL;

                    return Symbol(Symbol::SymbolType::AND.getString(), Symbol::SymbolType::AND);
                }

                lastSymbolType = Symbol::SymbolType::LITHERAL;
                return Symbol(symbolString, Symbol::SymbolType::LITHERAL);
            }

            while (true) {

                if (!((*position).isLetterOrNumber() || *position == '_') || position == line.end()) {
                    break;
                }
                symbolString += *position;

                position++;
            }

            if (lastSymbolType == Symbol::SymbolType::LITHERAL) {
                lastString = symbolString;
                lastSymbolType = Symbol::SymbolType::OTHER;

                return Symbol(Symbol::SymbolType::AND.getString(), Symbol::SymbolType::AND);
            }

            lastSymbolType = Symbol::SymbolType::OTHER;
            return Symbol(symbolString, Symbol::SymbolType::OTHER);
        }

        throw WarningExeption("There is no symbols.");
    }

    QString SyntaxTreeFactory::toPostfixString(const QString& line) const throws(AnalyzeCrashExeption) {
        SymbolFactory symbolFactory = SymbolFactory(line);
        QStack<Symbol::SymbolType> stack = QStack<Symbol::SymbolType> ();
        QString result = " ";


        try {
            while (true) {
                //qDebug(result.toAscii());
                
                Symbol symbol = symbolFactory.getNextSymbol();

                if (symbol.getType() == Symbol::SymbolType::OTHER || symbol.getType() == Symbol::SymbolType::LITHERAL) {
                    result += symbol.getString() + " ";
                } else {
                    while (!stack.empty() && stack.top().getPriority() >= symbol.getType().getPriority()) {
                        if (symbol.getType() == Symbol::SymbolType::CLOSE_BRACKET && stack.top() == Symbol::SymbolType::OPEN_BRACKET) {
                            stack.pop();
                        } else {
                            result += stack.pop().getString() + " ";
                        }
                    }
                    if (symbol.getType() != Symbol::SymbolType::CLOSE_BRACKET) {
                        stack.push(symbol.getType());
                    }
                }
            }
        } catch (WarningExeption) {
            return result;
        }

    }
}
#pragma once

#include <QString>
#include <QList>

#include "../Utils/Enum.h"
#include "../Core/Exeption.h"

#include "../defines.h"

namespace Core {

    /**
     * Класс, реализующий Token
     */
    class Symbol {
    public:

        struct SymbolTypeData {
            QChar symbol;
            unsigned int argsNumber;
            unsigned int priority;
            bool (*operation) (const QList<bool>& args);

            SymbolTypeData& operator =(const SymbolTypeData & that) {
                symbol = that.symbol;
                argsNumber = that.argsNumber;
                priority = that.priority;
                operation = that.operation;

                return *this;
            }

            bool operator ==(const SymbolTypeData & that) const {
                return symbol == that.symbol && argsNumber == that.argsNumber && priority == that.priority;
            }

            bool operator !=(const SymbolTypeData & that) const {
                return symbol != that.symbol || argsNumber != that.argsNumber || priority != that.priority;
            }

        };

        class SymbolType : public Enum<Symbol::SymbolTypeData> {
        protected:
            SymbolType(const QChar& symbol, bool (*operation) (const QList<bool>& args), unsigned int argsNumber, unsigned int priority = ~0u);

        public:
            SymbolType();

            static const SymbolType CLOSE_BRACKET;
            static const SymbolType OPEN_BRACKET;
            static const SymbolType OR;
            static const SymbolType AND;
            static const SymbolType LITHERAL;
            static const SymbolType IDENTYFIER;
            static const SymbolType SPACE;
            static const SymbolType BACKSLASH;

            inline bool operation(const QList<bool>& args) const {
                return value.operation(args);
            }

            inline const QChar& toChar() const {
                return value.symbol;
            }

            inline unsigned int getArgsNumber() const {
                return value.argsNumber;
            }

            inline unsigned int getPriority() const {
                return value.priority;
            }

        private:
            static bool andOperation(const QList<bool>& args);
            static bool orOperation(const QList<bool>& args);
            static bool equalOperation(const QList<bool>& args);
            static bool litheralOperation(const QList<bool>& args);
            static bool otherOperation(const QList<bool>& args);
        };

    protected:
        QString representation;
        SymbolType type;
        QString id;

    public:
        Symbol();
        Symbol(const QString& representation, const Symbol::SymbolType& type);
        Symbol(const Symbol::SymbolType& type);

        inline const QString& getRepresentation() const {
            return representation;
        }

        inline const SymbolType& getType() const {
            return type;
        }

        inline void setId(const QString& id) {
            this->id = id;
        }

        inline const QString& getId() const {
            return id;
        }

        inline bool operation(const QList<bool>& args) {
            return type.operation(args);
        }

        inline QString toString() const {
            return representation + "{@" + id + "}";
        }

        inline bool operator==(const Symbol& that) const {
            return type == that.type
                    && representation == that.representation;
        }

    };

    /**
     * Класс, реализующий Lexer, т.е. класс разбивающий входную строку на Token'ы
     */
    class SymbolFactory {
    private:
        QString line;
        QString::ConstIterator position;
        QString::ConstIterator end;

        QString lastString;
        Symbol::SymbolType lastSymbolType;

    public:
        SymbolFactory(const QString& string);

        Symbol getNextSymbol() throws(AnalyzeCrashExeption, WarningExeption);
        bool isNextSymbol();

    private:
        void skipFirst(const Symbol::SymbolType& symbolType);
        void skipAll(const Symbol::SymbolType& symbolType);
    };

}
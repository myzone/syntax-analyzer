#pragma once

#include <QString>
#include <QList>

#include "../Utils/Enum.h"
#include "../Core/Exeption.h"

#include "../defines.h"

namespace Core {

    class Symbol {
    public:

        struct SymbolTypeData {
            QString symbol;
            unsigned int argsNumber;
            unsigned int priority;
            bool (*operation) (const QList<bool>& args);

            bool operator ==(const SymbolTypeData & that) const {
                return symbol == that.symbol && argsNumber == that.argsNumber && priority == that.priority;
            }

            bool operator !=(const SymbolTypeData & that) const {
                return symbol != that.symbol || argsNumber != that.argsNumber || priority != that.priority;
            }

        };

        class SymbolType : public Enum<Symbol::SymbolTypeData> {
        protected:
            SymbolType(const QString& symbol, bool (*operation) (const QList<bool>& args), unsigned int argsNumber, unsigned int priority = ~0u);

        public:
            SymbolType();

            static const SymbolType CLOSE_BRACKET;
            static const SymbolType OPEN_BRACKET;
            static const SymbolType OR;
            static const SymbolType AND;

            static const SymbolType LITHERAL;
            static const SymbolType IDENTYFIER;

            static const SymbolType SPACE;
            static const SymbolType LINE_END;

            static const SymbolType BACKSLASH;
            static const SymbolType DEFINE;
            static const SymbolType DEFINE_END;

            inline bool operation(const QList<bool>& args) const {
                return value.operation(args);
            }

            inline const QString& toString() const {
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
            return type == that.type && representation == that.representation;
        }

    };

    class SymbolFactory {
    protected:
        QString source;
        QString::ConstIterator sourcePosition;
        QString::ConstIterator sourceEnd;

        QString lastString;
        Symbol::SymbolType lastSymbolType;

    public:
        SymbolFactory(const QString& string);

        virtual Symbol getNextSymbol() throws(AnalyzeCrashExeption, WarningExeption);
        virtual bool isNextSymbol() const;

    protected:
        void skipFirst(const Symbol::SymbolType& symbolType);
        void skipAll(const Symbol::SymbolType& symbolType);
        void skipAll(const Symbol::SymbolType& symbolTypeA, const Symbol::SymbolType& symbolTypeB);
        void skipAll(const Symbol::SymbolType& symbolTypeA, const Symbol::SymbolType& symbolTypeB, const Symbol::SymbolType& symbolTypeC);

        bool whetherNextSymbol(const Symbol::SymbolType& symbolType) const;
    };

}
#pragma once

#include <QString>

#include "../Utils/Tree.h"
#include "../Events/EventBroadcaster.h"
#include "Exeption.h"

#include "defines.h"

namespace Core {

    class Symbol {
    public:

        struct SymbolTypeData {
            QChar symbol;
            int priority;
            
            SymbolTypeData& operator =(const SymbolTypeData& that) {
                symbol = that.symbol;
                priority = that.priority;
                
                return *this;
            }
            
            bool operator ==(const SymbolTypeData& that) const {
                return symbol == that.symbol && priority == that.priority;
            }
            
            bool operator !=(const SymbolTypeData& that) const {
                return symbol != that.symbol || priority != that.priority;
            }

        };

        class SymbolType : public Enum<Symbol::SymbolTypeData> {
        protected:
            SymbolType(const QChar& symbol, int priority);

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

            inline const QChar& toChar() const {
                return value.symbol;
            }

            inline int getPriority() const {
                return value.priority;
            }
        };
    protected:
        QString string;
        SymbolType type;

    public:
        Symbol();
        Symbol(const QString& string, const Symbol::SymbolType& type);
        Symbol(const Symbol::SymbolType& type);

        const QString& getString() const;
        const SymbolType& getType() const;
    };

    class SymbolFactory {
    private:
        QString line;
        QString::ConstIterator position;
        
        QString lastString;
        Symbol::SymbolType lastSymbolType;
    public:
        SymbolFactory(const QString& string);
        SymbolFactory(const SymbolFactory& string);

        Symbol getNextSymbol() throws(AnalyzeCrashExeption, WarningExeption);
        bool isNextSymbol();
    private:
        void skipFirst(const Symbol::SymbolType& symbolType);
        void skipAll(const Symbol::SymbolType& symbolType);
    };

    class SyntaxTreeFactory {
    private:
        SyntaxTreeFactory() {}
        
    public:
        static Tree<QString> createTree(const QString& text) throws(AnalyzeCrashExeption);
        
    private:
        static void processLine(const QString& line, Tree<QString>& tree) throws(AnalyzeCrashExeption);
        static QList<QString> toSymbolList(const QString& line) throws(AnalyzeCrashExeption);
    
    };


}
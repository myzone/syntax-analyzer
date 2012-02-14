#pragma once

#include <QString>

#include "../Utils/Tree.h"
#include "../Events/EventBroadcaster.h"
#include "Exeption.h"

#define throws throw

namespace Core {

    class Symbol {
    public:

        struct SymbolTypeData {
            QString string;
            int priority;
            
            const SymbolTypeData& operator =(const SymbolTypeData& that) {
                string = that.string;
                priority = that.priority;
                
                return *this;
            }
            
            bool operator ==(const SymbolTypeData& that) const {
                return string == that.string && priority == that.priority;
            }
            
            bool operator !=(const SymbolTypeData& that) const {
                return string != that.string || priority != that.priority;
            }

        };

        class SymbolType : public Enum<Symbol::SymbolTypeData> {
        protected:
            SymbolType(const QString string, int priority);

        public:
            SymbolType();
            SymbolType(const SymbolType& that);
            
            static const SymbolType CLOSE_BRACKET;
            static const SymbolType OPEN_BRACKET;
            static const SymbolType OR;
            static const SymbolType AND;
            static const SymbolType LITHERAL;
            static const SymbolType OTHER;

            inline const QString& getString() const {
                return value.string;
            }

            inline int getPriority() const {
                return value.priority;
            }
        };
    protected:
        QString string;
        SymbolType type;

    public:
        Symbol(const QString& string, const Symbol::SymbolType& type);
        Symbol(const Symbol& that);

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
    };

    class SyntaxTreeFactory {
    public:

    //private:
        QString toPostfixString(const QString& line) const throws(AnalyzeCrashExeption);

    };


}
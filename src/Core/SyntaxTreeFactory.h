#pragma once

#include <QString>

#include "../Utils/Tree.h"
#include "../Events/EventBroadcaster.h"
#include "../Core/Exeption.h"

#include "../defines.h"

#include <iostream>

namespace Core {

    /*
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
            static bool AndOperation(const QList<bool>& args);
            static bool OrOperation(const QList<bool>& args);
            static bool EqualOperation(const QList<bool>& args);
            static bool OtherOperation(const QList<bool>& args);
        };

    protected:
        QString string;
        SymbolType type;
        QString id;

    public:
        Symbol();
        Symbol(const Symbol& that);
        Symbol(const QString& string, const Symbol::SymbolType& type);
        Symbol(const Symbol::SymbolType& type);

        const QString& toString() const;
        const SymbolType& getType() const;

        inline void setId(const QString& id) {
            this->id = id;
        }

        inline const QString& getId() {
            return id;
        }
    };

    /*
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

    /*
     * Класс реализующий семантический анализ
     */
    class SyntaxTreeFactory {
    private:

        Events::EventBroadcaster* broadcaster;

        SyntaxTreeFactory() {
        }

        class TreeProcessor : public Tree<Symbol>::DataProcessor {
        private:
            QMap<QString, QString>* map;
            bool ended;
            Tree<Symbol> node;
        public:

            TreeProcessor(QMap<QString, QString>* map) : map(map) {
            }

            virtual void dataProcessingStarts() {
                ended = true;
            }

            virtual void processData(Tree<Symbol>& nodeProvider) throws(Tree<Symbol>::TraverseStoppedExeption) {
                if (!nodeProvider.isLeaf()) return;

                if (nodeProvider.get().getType() == Symbol::SymbolType::IDENTYFIER) {
                    if (map->contains(nodeProvider.get().toString())) {
                        node = nodeProvider;
                        ended = false;
                        stop();
                    }
                }
            }

            virtual const TraverseType& getTraverseType() const {
                return TraverseType::WIDTH_TRAVERSE;
            }

            const Tree<Symbol>& getNode() const {
                return node;
            }

            bool isEnd() const {
                return ended;
            }
        };

    public:
        SyntaxTreeFactory(Events::EventBroadcaster* broadcaster);
        Tree<Symbol> createTree(const QString& text) const throws(AnalyzeCrashExeption);

    private:
        QMap<QString, QString> createLinesMap(const QString& text) const;
        void processLine(const QString& line, Tree<Symbol> tree) const throws(AnalyzeCrashExeption);
        QList<Symbol> toPostfixSymbolsList(const QString & line) const throws(AnalyzeCrashExeption);

    };
}

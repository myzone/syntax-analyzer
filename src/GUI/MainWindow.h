#pragma once

#include <QMainWindow>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QMap>

#include "../Events/EventListener.h"
#include "../Core/Analyzer.h"
#include "../GUI/TextTabWidget.h"

namespace GUI {

    class SyntaxHighlighter : public QSyntaxHighlighter, public Events::EventListener {
        Q_OBJECT
    private:
        class FormatType : public Enum<QTextCharFormat> {
        protected:
            FormatType(const QTextCharFormat& format);

        public:
            FormatType();

            static const FormatType KEYWORD_FORMAT;
            static const FormatType KEYWORD_SYMBOLS_FORMAT;
            static const FormatType QUOTED_STRING_FORMAT;
            static const FormatType COMMENT_FORMAT;
            static const FormatType WRONG_SYMBOL_FORMAT;
            static const FormatType WARNING_SYMBOL_FORMAT;

        protected:
            static QTextCharFormat createKeywordFormat();
            static QTextCharFormat createKeywordSymbolFormat();
            static QTextCharFormat createQuotedStringFormat();
            static QTextCharFormat createCommentFormat();
            static QTextCharFormat createWrongSymbolFormat();
            static QTextCharFormat createWarningSymbolFormat();

        };

    public:
        enum HighlightBlockState {
            NORMAL_STATE = -1,
            IN_LITHERAL,
            IN_MULTILINE_COMMENT
        };

        class HighlightRule {
        private:
            QRegExp pattern;
            QTextCharFormat format;

            HighlightBlockState neededState, resultState;

        public:
            HighlightRule(const QString& pattern, const QTextCharFormat& format);
            HighlightRule(const QString& pattern, const QTextCharFormat& format, const HighlightBlockState& neededState, const HighlightBlockState& resultState);

            bool isValid(const HighlightBlockState& state) const;

            const QRegExp& getPattern() const;
            const QTextCharFormat& getFormat() const;
            const HighlightBlockState& getResultState() const;
        };

        static const QList<HighlightRule> staticRules;
        QList<HighlightRule> dymamicRules;

        QTimer rehighlightTimer;
    public:
        SyntaxHighlighter(QTextDocument* parent = 0);

        void handle(const Events::AnalysingWasStartedEvent& event);
        void handle(const Events::AnalysingWasEndedEvent& event);

        void handle(const Events::SymbolIsNotUsedErrorEvent& event);

        void handle(const Events::SymbolIsNotDefinedErrorEvent& event);
        void handle(const Events::SymbolHasMistakeErrorEvent& event);
        void handle(const Events::LibraryFileCannotBeFoundErrorEvent& event);
        void handle(const Events::LibraryFileHasMistakeErrorEvent& event);
        void handle(const Events::SymbolIsNotClosedErrorEvent& event);
        void handle(const Events::DoubleDefenitionErrorEvent& event);
        void handle(const Events::WrongSymbolDefinitionErrorEvent& event);
        void handle(const Events::WrongBracketsNumberErrorEvent& event);

    protected:
        void highlightBlock(const QString& text);
        void highlightBlockWithRulesList(const QString& text, const QList<HighlightRule>& rulesList);

        static QList<HighlightRule> initStaticRules();
    };

    class MainWindow : public QMainWindow {
        Q_OBJECT

    private:
        QMap<QTextEdit*, QString> filesMap;
        QMap<QTextEdit*, Core::Analyzer*> analyzersMap;
        TextTabWidget* tabsWidget;

        QTimer analyzeTimer;
    public:
        explicit MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
        virtual ~MainWindow();

    private slots:
        void saveFile();
        void saveFileAs();
        void openFile();

        void analyze();

        void initTextEdit(QTextEdit* textEdit);
    };

}
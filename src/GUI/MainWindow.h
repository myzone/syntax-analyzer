#pragma once

#include <QMainWindow>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QMap>

#include "../Events/EventListener.h"
#include "../Core/Analyzer.h"
#include "../GUI/TextTabWidget.h"

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
        static const FormatType WRONG_SYMBOL_FORMAT;
        static const FormatType WARNING_SYMBOL_FORMAT;

    protected:
        static QTextCharFormat createKeywordFormat();
        static QTextCharFormat createKeywordSymbolFormat();
        static QTextCharFormat createQuotedStringFormat();
        static QTextCharFormat createWrongSymbolFormat();
        static QTextCharFormat createWarningSymbolFormat();
    };

    struct HighlightRule {
        HighlightRule(const QString& pattern, const QTextCharFormat& format) : pattern(QRegExp(pattern)), format(format) { }

        QRegExp pattern;
        QTextCharFormat format;
    };

    static const QList<HighlightRule> staticRules;
    QList<HighlightRule> dymamicRules;

    QTimer rehighlightTimer;
public:
    SyntaxHighlighter(QTextDocument *parent = 0);

    void handle(const Events::AnalysingWasStartedEvent& event);
    void handle(const Events::AnalysingWasEndedEvent& event);
    
    void handle(const Events::SymbolIsNotDefinedErrorEvent& event);
    void handle(const Events::SymbolHasMistakeErrorEvent& event);
    void handle(const Events::LibraryFileCannotBeFoundErrorEvent& event);
    void handle(const Events::LitheralIsNotClosedErrorEvent& event);
    void handle(const Events::DoubleDefenitionErrorEvent& event);
    
    void handle(const Events::SymbolIsNotUsedErrorEvent& event);
    
protected:
    void highlightBlock(const QString &text);

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
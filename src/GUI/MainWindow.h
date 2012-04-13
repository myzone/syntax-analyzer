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

    protected:
        static QTextCharFormat createKeywordFormat();
        static QTextCharFormat createKeywordSymbolFormat();
        static QTextCharFormat createQuotedStringFormat();
        static QTextCharFormat createWrongSymbolFormat();
    };

    struct HighlightRule {
        HighlightRule(const QString& pattern, const QTextCharFormat & format) : pattern(QRegExp(pattern)), format(format) { }

        QRegExp pattern;
        QTextCharFormat format;
    };

    static const QList<HighlightRule> staticRules;

public:
    SyntaxHighlighter(QTextDocument *parent = 0);

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
public:
    explicit MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~MainWindow();

private slots:
    void saveFile();
    void saveFileAs();
    void openFile();

    void analyze();

    void initTextEdit(QTextEdit* textEdit);
};
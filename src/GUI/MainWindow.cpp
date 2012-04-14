#include <QMenu>
#include <QMenuBar>

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QBrush>
#include <QTimer>
#include <QKeySequence>

#include "../GUI/MainWindow.h"

const SyntaxHighlighter::FormatType SyntaxHighlighter::FormatType::KEYWORD_FORMAT = SyntaxHighlighter::FormatType(createKeywordFormat());
const SyntaxHighlighter::FormatType SyntaxHighlighter::FormatType::KEYWORD_SYMBOLS_FORMAT = SyntaxHighlighter::FormatType(createKeywordSymbolFormat());
const SyntaxHighlighter::FormatType SyntaxHighlighter::FormatType::QUOTED_STRING_FORMAT = SyntaxHighlighter::FormatType(createQuotedStringFormat());
const SyntaxHighlighter::FormatType SyntaxHighlighter::FormatType::WRONG_SYMBOL_FORMAT = SyntaxHighlighter::FormatType(createWrongSymbolFormat());
const SyntaxHighlighter::FormatType SyntaxHighlighter::FormatType::WARNING_SYMBOL_FORMAT = SyntaxHighlighter::FormatType(createWarningSymbolFormat());

SyntaxHighlighter::FormatType::FormatType() : Enum<QTextCharFormat>() { }

SyntaxHighlighter::FormatType::FormatType(const QTextCharFormat& format) : Enum<QTextCharFormat>(format) { }

const QList<SyntaxHighlighter::HighlightRule> SyntaxHighlighter::staticRules = SyntaxHighlighter::initStaticRules();

QTextCharFormat SyntaxHighlighter::FormatType::createKeywordFormat() {
    QTextCharFormat format = QTextCharFormat();

    format.setForeground(Qt::darkBlue);
    format.setFontWeight(QFont::Bold);

    return format;
}

void SyntaxHighlighter::handle(const Events::AnalysingWasStartedEvent& event) {
    dymamicRules.clear();
}

void SyntaxHighlighter::handle(const Events::AnalysingWasEndedEvent& event) { }

void SyntaxHighlighter::handle(const Events::SymbolIsNotDefinedErrorEvent& event) {
    dymamicRules.append(HighlightRule("\\b" + event.getRepresentation() + "\\b", FormatType::WRONG_SYMBOL_FORMAT));
}

void SyntaxHighlighter::handle(const Events::SymbolHasMistakeErrorEvent& event) {
    dymamicRules.append(HighlightRule("\\b" + event.getRepresentation() + "\\b", FormatType::WRONG_SYMBOL_FORMAT));
}

void SyntaxHighlighter::handle(const Events::LibraryFileCannotBeFoundErrorEvent& event) {
    dymamicRules.append(HighlightRule("\\b" + event.getRepresentation() + "\\b", FormatType::WRONG_SYMBOL_FORMAT));

}

void SyntaxHighlighter::handle(const Events::LitheralIsNotClosedErrorEvent& event) {
    qDebug(event.getRepresentation().toAscii());
    dymamicRules.append(HighlightRule(event.getRepresentation(), FormatType::WRONG_SYMBOL_FORMAT));

}

void SyntaxHighlighter::handle(const Events::DoubleDefenitionErrorEvent& event) {
    dymamicRules.append(HighlightRule("\\b" + event.getRepresentation() + "\\b", FormatType::WRONG_SYMBOL_FORMAT));
}

void SyntaxHighlighter::handle(const Events::SymbolIsNotUsedErrorEvent& event) {
    dymamicRules.append(HighlightRule("\\b" + event.getRepresentation() + "\\b", FormatType::WARNING_SYMBOL_FORMAT));
}

QTextCharFormat SyntaxHighlighter::FormatType::createKeywordSymbolFormat() {
    QTextCharFormat format = QTextCharFormat();

    format.setFontWeight(QFont::Bold);

    return format;
}

QTextCharFormat SyntaxHighlighter::FormatType::createQuotedStringFormat() {
    QTextCharFormat format = QTextCharFormat();

    format.setForeground(QBrush(QColor::fromRgb(180, 20, 100, 255)));

    return format;
}

QTextCharFormat SyntaxHighlighter::FormatType::createWrongSymbolFormat() {
    QTextCharFormat format = QTextCharFormat();

    format.setUnderlineColor(Qt::red);
    format.setUnderlineStyle(QTextCharFormat::WaveUnderline);

    return format;
}

QTextCharFormat SyntaxHighlighter::FormatType::createWarningSymbolFormat() {
    QTextCharFormat format = QTextCharFormat();

    format.setUnderlineColor(Qt::yellow);
    format.setUnderlineStyle(QTextCharFormat::WaveUnderline);

    return format;
}

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent), rehighlightTimer(this) {
    connect(&rehighlightTimer, SIGNAL(timeout()), this, SLOT(rehighlight()));
    rehighlightTimer.start(500);
}

void SyntaxHighlighter::highlightBlock(const QString& text) {

    for (QList<SyntaxHighlighter::HighlightRule>::ConstIterator it = dymamicRules.constBegin(), end = dymamicRules.constEnd(); it != end; ++it) {
        QRegExp expression = (*it).pattern;
        
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            if(length <= 0) 
                break;
            
            QTextCharFormat temp = (*it).format;
            temp.merge(format(index));
            
            setFormat(index, length, temp);

            index = expression.indexIn(text, index + length);
        }
    }

    for (QList<SyntaxHighlighter::HighlightRule>::ConstIterator it = staticRules.constBegin(), end = staticRules.constEnd(); it != end; ++it) {
        QRegExp expression = (*it).pattern;
        
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            if(length <= 0) 
                break;
            
            QTextCharFormat temp = (*it).format;
            temp.merge(format(index));
            
            setFormat(index, length, temp);

            index = expression.indexIn(text, index + length);
        }
    }
}

QList<SyntaxHighlighter::HighlightRule> SyntaxHighlighter::initStaticRules() {
    QList<SyntaxHighlighter::HighlightRule> rules = QList<SyntaxHighlighter::HighlightRule > ();

    rules.append(SyntaxHighlighter::HighlightRule("\\bimport\\b", FormatType::KEYWORD_FORMAT));

    rules.append(SyntaxHighlighter::HighlightRule("->", FormatType::KEYWORD_SYMBOLS_FORMAT));
    rules.append(SyntaxHighlighter::HighlightRule("\\(", FormatType::KEYWORD_SYMBOLS_FORMAT));
    rules.append(SyntaxHighlighter::HighlightRule("\\)", FormatType::KEYWORD_SYMBOLS_FORMAT));
    rules.append(SyntaxHighlighter::HighlightRule("\\|", FormatType::KEYWORD_SYMBOLS_FORMAT));
    rules.append(SyntaxHighlighter::HighlightRule("\\&", FormatType::KEYWORD_SYMBOLS_FORMAT));
    rules.append(SyntaxHighlighter::HighlightRule(";", FormatType::KEYWORD_SYMBOLS_FORMAT));

    rules.append(SyntaxHighlighter::HighlightRule("\"([^\"]|(\\\\\"))*\"?", FormatType::QUOTED_STRING_FORMAT));

    return rules;
}

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags flags) : QMainWindow(parent, flags), filesMap(), analyzersMap(), analyzeTimer(this) {

    setWindowTitle(trUtf8("Cинтаксический анализатор"));

    QMenuBar* menuBar = new QMenuBar();
    QMenu* fileMenu = new QMenu(trUtf8("Файл"));
    QMenu* toolsMenu = new QMenu(trUtf8("Инструменты"));

    QAction* openFileAction = new QAction(trUtf8("Открыть файл..."), fileMenu);
    QAction* saveFileAction = new QAction(trUtf8("Сохранить файл"), fileMenu);
    QAction* saveFileAsAction = new QAction(trUtf8("Сохранить файл как..."), fileMenu);
    QAction* exitAction = new QAction(trUtf8("Выход"), fileMenu);

    QAction* analyzeAction = new QAction(trUtf8("Проверить"), toolsMenu);

    openFileAction->setShortcut(QKeySequence::Open);
    saveFileAction->setShortcut(QKeySequence::Save);
    saveFileAsAction->setShortcut(QKeySequence::SaveAs);
    exitAction->setShortcut(QKeySequence::Quit);

    analyzeAction->setShortcut(QKeySequence(Qt::Key_F6));

    connect(openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(saveFileAsAction, SIGNAL(triggered()), this, SLOT(saveFileAs()));
    connect(saveFileAction, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    connect(analyzeAction, SIGNAL(triggered()), this, SLOT(analyze()));

    fileMenu->addAction(openFileAction);
    fileMenu->addAction(saveFileAction);
    fileMenu->addAction(saveFileAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    toolsMenu->addAction(analyzeAction);

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(toolsMenu);

    tabsWidget = new TextTabWidget();

    connect(tabsWidget, SIGNAL(onTabAdded(QTextEdit*)), this, SLOT(initTextEdit(QTextEdit*)));

    tabsWidget->addTextTab();

    setMenuBar(menuBar);
    setCentralWidget(tabsWidget);
}

MainWindow::~MainWindow() {

    delete tabsWidget;
}

void MainWindow::openFile() {
    QString pathToFile = QFileDialog::getOpenFileName(
            this,
            trUtf8("Открыть файл"),
            QDir::currentPath(),
            trUtf8("Файлы грамматик(*.lng)")
            );

    if (pathToFile == "") return;

    QFile file(pathToFile);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, trUtf8("Ошибка отрытия файла"), trUtf8("Невозможно открыть файл"));

        return;
    }

    QTextEdit* newTextEdit = tabsWidget->addTextTab();
    newTextEdit->setText(file.readAll());
    file.close();

    filesMap.insert(newTextEdit, pathToFile);
    tabsWidget->renameTab(tabsWidget->indexOf(newTextEdit), pathToFile.replace(QRegExp("^([A-Z]:)?((.*)(/|\\\\))*"), ""));
}

void MainWindow::saveFile() {
    QTextEdit* currentTextEdit = tabsWidget->getTab(tabsWidget->getCurrentTabIndex());

    if (!filesMap.contains(currentTextEdit)) {
        saveFileAs();
        return;
    }

    if (tabsWidget->isTabSaved(tabsWidget->getCurrentTabIndex())) return;

    QFile file(filesMap.value(currentTextEdit));

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, trUtf8("Ошибка отрытия файла"), trUtf8("Невозможно открыть файл"));

        return;
    }

    file.write(currentTextEdit->toPlainText().toAscii());
    file.close();

    tabsWidget->markTabAsSaved(tabsWidget->getCurrentTabIndex());
}

void MainWindow::saveFileAs() {
    QTextEdit* currentTextEdit = tabsWidget->getTab(tabsWidget->getCurrentTabIndex());

    QString pathToFile = QFileDialog::getSaveFileName(
            this,
            trUtf8("Открыть файл"),
            QDir::currentPath(),
            trUtf8("Файлы грамматик(*.lng)")
            );


    if (pathToFile == "") return;

    QFile file(pathToFile);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, trUtf8("Ошибка отрытия файла"), trUtf8("Невозможно открыть файл"));

        return;
    }

    file.write(currentTextEdit->toPlainText().toAscii());
    file.close();

    tabsWidget->markTabAsSaved(tabsWidget->getCurrentTabIndex());
    tabsWidget->renameTab(tabsWidget->indexOf(currentTextEdit), pathToFile.replace(QRegExp("^((.*)/)*"), ""));
    filesMap.insert(currentTextEdit, pathToFile);
}

void MainWindow::analyze() {

    QTextEdit* target = tabsWidget->getTab(tabsWidget->getCurrentTabIndex());

    analyzersMap.value(target)->analyze(target->toPlainText());
}

void MainWindow::initTextEdit(QTextEdit* textEdit) {
    const QString PATH_TO_LIB = "/home/myzone/Рабочий стол/";

    SyntaxHighlighter* syntaxHighlighter = new SyntaxHighlighter(textEdit->document());

    Core::Analyzer* analyzer = new Core::Analyzer(PATH_TO_LIB);
    analyzer->addErrorEventListener(syntaxHighlighter);
    analyzersMap.insert(textEdit, analyzer);

    tabsWidget->markTabAsSaved(tabsWidget->indexOf(textEdit));

    connect(&analyzeTimer, SIGNAL(timeout()), this, SLOT(analyze()));
    analyzeTimer.start(1000);
}
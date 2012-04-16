#include <QTabBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QAction>
#include <QTimer>

#include "../GUI/TextTabWidget.h"

namespace GUI {

    TextTabWidget::TextTabWidget(QWidget* parent) : QTabWidget(parent) {
        setTabsClosable(true);

        nullWidgetForAddButton = new QWidget();

        QPalette palette(nullWidgetForAddButton->palette());
        palette.setColor(QPalette::Background, Qt::gray);

        nullWidgetForAddButton->setPalette(palette);
        nullWidgetForAddButton->setStyleSheet("background-color:lightgray;");
        addTab(nullWidgetForAddButton, "+");

        tabBar()->setTabButton(0, QTabBar::RightSide, 0);
        tabBar()->installEventFilter(this);

        QAction* openTabAction = new QAction(this);
        openTabAction->setShortcut(QKeySequence::AddTab);

        lastTabIndex = 0;
        lastTabText = "";

        connect(openTabAction, SIGNAL(triggered()), this, SLOT(addTabSlot()));
        connect(this, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
        connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(confirmRemovingTab(int)));
    }

    TextTabWidget::~TextTabWidget() {
        while (count())
            delete widget(0);
    }

    QTextEdit* TextTabWidget::addTextTab() {
        QTextEdit* textEdit = new QTextEdit();

        int tabsCount = count();
        insertTab(tabsCount - 1, textEdit, tr("undefined"));
        setCurrentWidget(textEdit);

        QTimer* checkTimer = new QTimer(this);
        connect(checkTimer, SIGNAL(timeout()), this, SLOT(updateTextEditState()));
        checkTimer->start(500);

        emit onTabAdded(textEdit);

        return textEdit;
    }

    void TextTabWidget::markTabAsSaved(int tabIndex) {
        QTextEdit* target = (QTextEdit*) widget(tabIndex);

        if (editedTextEdits.contains(target)) {
            QString tabName = tabText(indexOf(target));
            setTabText(indexOf(target), tabName.left(tabName.length() - 1));
            editedTextEdits.remove(target);
        }
    }

    bool TextTabWidget::isTabSaved(int tabIndex) {
        QTextEdit* target = (QTextEdit*) widget(tabIndex);

        return !editedTextEdits.contains(target);
    }

    void TextTabWidget::renameTab(int tabIndex, const QString& newTabName) {
        setTabText(tabIndex, newTabName);
    }

    QTextEdit* TextTabWidget::getTab(int tabIndex) {
        return (QTextEdit*) widget(tabIndex);
    }

    int TextTabWidget::getCurrentTabIndex() {
        return currentIndex();
    }

    bool TextTabWidget::eventFilter(QObject* object, QEvent* event) {
        QMouseEvent* mouseEvent = (QMouseEvent*) event;
        if (nullWidgetForAddButton == widget(tabBar()->tabAt(mouseEvent->pos())) && event->type() == QEvent::MouseButtonPress) {
            setCurrentWidget(addTextTab());
            return true;
        }

        return QTabWidget::eventFilter(object, event);
    }

    void TextTabWidget::confirmRemovingTab(int tabIndex) {
        bool willBeClosed = true;

        QTextEdit* target = (QTextEdit*) widget(tabIndex);

        if (editedTextEdits.contains(target)) {
            QMessageBox yesNoDialog;
            yesNoDialog.setWindowTitle(trUtf8("Подтвержение закрытия"));
            yesNoDialog.setText(trUtf8("Данные будут утеряны, продолжить?"));
            yesNoDialog.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            yesNoDialog.setDefaultButton(QMessageBox::Cancel);

            if (yesNoDialog.exec() == QMessageBox::Cancel) {
                willBeClosed = false;
            }
        }

        if (willBeClosed) {
            delete target;
        }

    }

    void TextTabWidget::updateTextEditState() {
        QTextEdit* target = (QTextEdit*) currentWidget();

        if (currentIndex() == lastTabIndex) {
            if (target->toPlainText() != lastTabText) {
                if (!editedTextEdits.contains(target)) {
                    setTabText(indexOf(target), tabText(indexOf(target)) + "*");
                    editedTextEdits.insert(target);
                }
            }
        }

        lastTabIndex = currentIndex();
        lastTabText = target->toPlainText();
    }

    void TextTabWidget::tabChanged(int tabIndex) {
        if (tabIndex == indexOf(nullWidgetForAddButton)) {
            if (count() == 1) {
                exit(0);
            } else {
                setCurrentIndex(tabIndex - 1);
            }
        }
    }

    void TextTabWidget::addTabSlot() {
        addTextTab();
    }

}
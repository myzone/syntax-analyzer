#pragma once

#include <QTabWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QSet>
#include <QEvent>

class TextTabWidget : public QTabWidget {
    Q_OBJECT

protected:
    QSet<QTextEdit*> editedTextEdits;
    QWidget* nullWidgetForAddButton;

public:
    explicit TextTabWidget(QWidget* parent = 0);
    virtual ~TextTabWidget();

    QTextEdit* addTextTab();
    
    void markTabAsSaved(int tabIndex);
    bool isTabSaved(int tabIndex);
    
    void renameTab(int tabIndex, const QString& newTabName);
                
    QTextEdit* getTab(int tabIndex);
    int getCurrentTabIndex();

    bool eventFilter(QObject* object, QEvent* event);

signals:
    void onTabAdded(QTextEdit* textEdit);

public slots:

protected slots:
    void addTabSlot();
    void tabChanged(int tabIndex);
    void updateTextEditState();
    void confirmRemovingTab(int tabIndex);
};


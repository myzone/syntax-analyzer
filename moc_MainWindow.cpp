/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created: Tue Apr 17 21:28:01 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/GUI/MainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GUI__SyntaxHighlighter[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_GUI__SyntaxHighlighter[] = {
    "GUI::SyntaxHighlighter\0"
};

const QMetaObject GUI::SyntaxHighlighter::staticMetaObject = {
    { &QSyntaxHighlighter::staticMetaObject, qt_meta_stringdata_GUI__SyntaxHighlighter,
      qt_meta_data_GUI__SyntaxHighlighter, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GUI::SyntaxHighlighter::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GUI::SyntaxHighlighter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GUI::SyntaxHighlighter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GUI__SyntaxHighlighter))
        return static_cast<void*>(const_cast< SyntaxHighlighter*>(this));
    if (!strcmp(_clname, "Events::EventListener"))
        return static_cast< Events::EventListener*>(const_cast< SyntaxHighlighter*>(this));
    return QSyntaxHighlighter::qt_metacast(_clname);
}

int GUI::SyntaxHighlighter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSyntaxHighlighter::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_GUI__MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x08,
      28,   16,   16,   16, 0x08,
      41,   16,   16,   16, 0x08,
      52,   16,   16,   16, 0x08,
      71,   62,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_GUI__MainWindow[] = {
    "GUI::MainWindow\0\0saveFile()\0saveFileAs()\0"
    "openFile()\0analyze()\0textEdit\0"
    "initTextEdit(QTextEdit*)\0"
};

const QMetaObject GUI::MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_GUI__MainWindow,
      qt_meta_data_GUI__MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GUI::MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GUI::MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GUI::MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GUI__MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int GUI::MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: saveFile(); break;
        case 1: saveFileAs(); break;
        case 2: openFile(); break;
        case 3: analyze(); break;
        case 4: initTextEdit((*reinterpret_cast< QTextEdit*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

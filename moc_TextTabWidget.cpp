/****************************************************************************
** Meta object code from reading C++ file 'TextTabWidget.h'
**
** Created: Mon Apr 16 00:57:12 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/GUI/TextTabWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TextTabWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GUI__TextTabWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      29,   20,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      52,   19,   19,   19, 0x09,
      74,   65,   19,   19, 0x09,
      90,   19,   19,   19, 0x09,
     112,   65,   19,   19, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_GUI__TextTabWidget[] = {
    "GUI::TextTabWidget\0\0textEdit\0"
    "onTabAdded(QTextEdit*)\0addTabSlot()\0"
    "tabIndex\0tabChanged(int)\0updateTextEditState()\0"
    "confirmRemovingTab(int)\0"
};

const QMetaObject GUI::TextTabWidget::staticMetaObject = {
    { &QTabWidget::staticMetaObject, qt_meta_stringdata_GUI__TextTabWidget,
      qt_meta_data_GUI__TextTabWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GUI::TextTabWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GUI::TextTabWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GUI::TextTabWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GUI__TextTabWidget))
        return static_cast<void*>(const_cast< TextTabWidget*>(this));
    return QTabWidget::qt_metacast(_clname);
}

int GUI::TextTabWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTabWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onTabAdded((*reinterpret_cast< QTextEdit*(*)>(_a[1]))); break;
        case 1: addTabSlot(); break;
        case 2: tabChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: updateTextEditState(); break;
        case 4: confirmRemovingTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void GUI::TextTabWidget::onTabAdded(QTextEdit * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

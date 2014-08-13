/****************************************************************************
** Meta object code from reading C++ file 'titlebar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DrawTitleBar/titlebar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'titlebar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TitleBar_t {
    QByteArrayData data[11];
    char stringdata[119];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_TitleBar_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_TitleBar_t qt_meta_stringdata_TitleBar = {
    {
QT_MOC_LITERAL(0, 0, 8),
QT_MOC_LITERAL(1, 9, 10),
QT_MOC_LITERAL(2, 20, 0),
QT_MOC_LITERAL(3, 21, 18),
QT_MOC_LITERAL(4, 40, 12),
QT_MOC_LITERAL(5, 53, 8),
QT_MOC_LITERAL(6, 62, 7),
QT_MOC_LITERAL(7, 70, 7),
QT_MOC_LITERAL(8, 78, 11),
QT_MOC_LITERAL(9, 90, 13),
QT_MOC_LITERAL(10, 104, 13)
    },
    "TitleBar\0signal_min\0\0signal_max_restore\0"
    "signal_close\0showSkin\0showMin\0showMax\0"
    "closeWidget\0slot_btnpress\0slot_btnclick\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TitleBar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06,
       3,    0,   60,    2, 0x06,
       4,    0,   61,    2, 0x06,
       5,    0,   62,    2, 0x06,
       6,    0,   63,    2, 0x06,
       7,    0,   64,    2, 0x06,
       8,    0,   65,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       9,    0,   66,    2, 0x0a,
      10,    0,   67,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TitleBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TitleBar *_t = static_cast<TitleBar *>(_o);
        switch (_id) {
        case 0: _t->signal_min(); break;
        case 1: _t->signal_max_restore(); break;
        case 2: _t->signal_close(); break;
        case 3: _t->showSkin(); break;
        case 4: _t->showMin(); break;
        case 5: _t->showMax(); break;
        case 6: _t->closeWidget(); break;
        case 7: _t->slot_btnpress(); break;
        case 8: _t->slot_btnclick(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TitleBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TitleBar::signal_min)) {
                *result = 0;
            }
        }
        {
            typedef void (TitleBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TitleBar::signal_max_restore)) {
                *result = 1;
            }
        }
        {
            typedef void (TitleBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TitleBar::signal_close)) {
                *result = 2;
            }
        }
        {
            typedef void (TitleBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TitleBar::showSkin)) {
                *result = 3;
            }
        }
        {
            typedef void (TitleBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TitleBar::showMin)) {
                *result = 4;
            }
        }
        {
            typedef void (TitleBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TitleBar::showMax)) {
                *result = 5;
            }
        }
        {
            typedef void (TitleBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TitleBar::closeWidget)) {
                *result = 6;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject TitleBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TitleBar.data,
      qt_meta_data_TitleBar,  qt_static_metacall, 0, 0}
};


const QMetaObject *TitleBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TitleBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TitleBar.stringdata))
        return static_cast<void*>(const_cast< TitleBar*>(this));
    return QWidget::qt_metacast(_clname);
}

int TitleBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void TitleBar::signal_min()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void TitleBar::signal_max_restore()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void TitleBar::signal_close()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void TitleBar::showSkin()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void TitleBar::showMin()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void TitleBar::showMax()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void TitleBar::closeWidget()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}
QT_END_MOC_NAMESPACE

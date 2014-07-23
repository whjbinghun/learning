/****************************************************************************
** Meta object code from reading C++ file 'vodplayinterface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VodPlayInterface/vodplayinterface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vodplayinterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VodPlayInterface_t {
    QByteArrayData data[18];
    char stringdata[361];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_VodPlayInterface_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_VodPlayInterface_t qt_meta_stringdata_VodPlayInterface = {
    {
QT_MOC_LITERAL(0, 0, 16),
QT_MOC_LITERAL(1, 17, 12),
QT_MOC_LITERAL(2, 30, 0),
QT_MOC_LITERAL(3, 31, 10),
QT_MOC_LITERAL(4, 42, 10),
QT_MOC_LITERAL(5, 53, 29),
QT_MOC_LITERAL(6, 83, 28),
QT_MOC_LITERAL(7, 112, 29),
QT_MOC_LITERAL(8, 142, 26),
QT_MOC_LITERAL(9, 169, 29),
QT_MOC_LITERAL(10, 199, 30),
QT_MOC_LITERAL(11, 230, 30),
QT_MOC_LITERAL(12, 261, 27),
QT_MOC_LITERAL(13, 289, 16),
QT_MOC_LITERAL(14, 306, 3),
QT_MOC_LITERAL(15, 310, 18),
QT_MOC_LITERAL(16, 329, 15),
QT_MOC_LITERAL(17, 345, 14)
    },
    "VodPlayInterface\0close_window\0\0"
    "min_window\0max_window\0"
    "sig_display_monitor_interface\0"
    "sig_display_config_interface\0"
    "sig_display_analyse_interface\0"
    "sig_display_back_interface\0"
    "slot_display_config_interface\0"
    "slot_display_monitor_interface\0"
    "slot_display_analyse_interface\0"
    "slot_display_back_interface\0"
    "show_contextmenu\0pos\0slot_show_max_size\0"
    "send_update_xml\0send_heart_tag\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VodPlayInterface[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x06,
       3,    0,   90,    2, 0x06,
       4,    0,   91,    2, 0x06,
       5,    0,   92,    2, 0x06,
       6,    0,   93,    2, 0x06,
       7,    0,   94,    2, 0x06,
       8,    0,   95,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       9,    0,   96,    2, 0x0a,
      10,    0,   97,    2, 0x0a,
      11,    0,   98,    2, 0x0a,
      12,    0,   99,    2, 0x0a,
      13,    1,  100,    2, 0x0a,
      15,    0,  103,    2, 0x0a,
      16,    0,  104,    2, 0x0a,
      17,    0,  105,    2, 0x0a,

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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void VodPlayInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VodPlayInterface *_t = static_cast<VodPlayInterface *>(_o);
        switch (_id) {
        case 0: _t->close_window(); break;
        case 1: _t->min_window(); break;
        case 2: _t->max_window(); break;
        case 3: _t->sig_display_monitor_interface(); break;
        case 4: _t->sig_display_config_interface(); break;
        case 5: _t->sig_display_analyse_interface(); break;
        case 6: _t->sig_display_back_interface(); break;
        case 7: _t->slot_display_config_interface(); break;
        case 8: _t->slot_display_monitor_interface(); break;
        case 9: _t->slot_display_analyse_interface(); break;
        case 10: _t->slot_display_back_interface(); break;
        case 11: _t->show_contextmenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 12: _t->slot_show_max_size(); break;
        case 13: _t->send_update_xml(); break;
        case 14: _t->send_heart_tag(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (VodPlayInterface::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VodPlayInterface::close_window)) {
                *result = 0;
            }
        }
        {
            typedef void (VodPlayInterface::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VodPlayInterface::min_window)) {
                *result = 1;
            }
        }
        {
            typedef void (VodPlayInterface::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VodPlayInterface::max_window)) {
                *result = 2;
            }
        }
        {
            typedef void (VodPlayInterface::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VodPlayInterface::sig_display_monitor_interface)) {
                *result = 3;
            }
        }
        {
            typedef void (VodPlayInterface::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VodPlayInterface::sig_display_config_interface)) {
                *result = 4;
            }
        }
        {
            typedef void (VodPlayInterface::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VodPlayInterface::sig_display_analyse_interface)) {
                *result = 5;
            }
        }
        {
            typedef void (VodPlayInterface::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VodPlayInterface::sig_display_back_interface)) {
                *result = 6;
            }
        }
    }
}

const QMetaObject VodPlayInterface::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_VodPlayInterface.data,
      qt_meta_data_VodPlayInterface,  qt_static_metacall, 0, 0}
};


const QMetaObject *VodPlayInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VodPlayInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VodPlayInterface.stringdata))
        return static_cast<void*>(const_cast< VodPlayInterface*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int VodPlayInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void VodPlayInterface::close_window()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void VodPlayInterface::min_window()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void VodPlayInterface::max_window()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void VodPlayInterface::sig_display_monitor_interface()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void VodPlayInterface::sig_display_config_interface()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void VodPlayInterface::sig_display_analyse_interface()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void VodPlayInterface::sig_display_back_interface()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}
QT_END_MOC_NAMESPACE

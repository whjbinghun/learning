/****************************************************************************
** Meta object code from reading C++ file 'wndvideoplayback.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VodPlayInterface/wndvideoplayback.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wndvideoplayback.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_WndVideoPlayback_t {
    QByteArrayData data[11];
    char stringdata[198];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_WndVideoPlayback_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_WndVideoPlayback_t qt_meta_stringdata_WndVideoPlayback = {
    {
QT_MOC_LITERAL(0, 0, 16),
QT_MOC_LITERAL(1, 17, 23),
QT_MOC_LITERAL(2, 41, 0),
QT_MOC_LITERAL(3, 42, 14),
QT_MOC_LITERAL(4, 57, 23),
QT_MOC_LITERAL(5, 81, 7),
QT_MOC_LITERAL(6, 89, 23),
QT_MOC_LITERAL(7, 113, 7),
QT_MOC_LITERAL(8, 121, 22),
QT_MOC_LITERAL(9, 144, 27),
QT_MOC_LITERAL(10, 172, 24)
    },
    "WndVideoPlayback\0slot_btn_start_or_pause\0"
    "\0slot_btn_strop\0slot_slider_play_change\0"
    "n_value\0slot_combo_change_group\0n_index\0"
    "slot_timer_connect_req\0"
    "slot_timer_playback_cmd_req\0"
    "slot_timer_update_slider\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WndVideoPlayback[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a,
       3,    0,   50,    2, 0x0a,
       4,    1,   51,    2, 0x0a,
       6,    1,   54,    2, 0x0a,
       8,    0,   57,    2, 0x0a,
       9,    0,   58,    2, 0x0a,
      10,    0,   59,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void WndVideoPlayback::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WndVideoPlayback *_t = static_cast<WndVideoPlayback *>(_o);
        switch (_id) {
        case 0: _t->slot_btn_start_or_pause(); break;
        case 1: _t->slot_btn_strop(); break;
        case 2: _t->slot_slider_play_change((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->slot_combo_change_group((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->slot_timer_connect_req(); break;
        case 5: _t->slot_timer_playback_cmd_req(); break;
        case 6: _t->slot_timer_update_slider(); break;
        default: ;
        }
    }
}

const QMetaObject WndVideoPlayback::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_WndVideoPlayback.data,
      qt_meta_data_WndVideoPlayback,  qt_static_metacall, 0, 0}
};


const QMetaObject *WndVideoPlayback::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WndVideoPlayback::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WndVideoPlayback.stringdata))
        return static_cast<void*>(const_cast< WndVideoPlayback*>(this));
    return QWidget::qt_metacast(_clname);
}

int WndVideoPlayback::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

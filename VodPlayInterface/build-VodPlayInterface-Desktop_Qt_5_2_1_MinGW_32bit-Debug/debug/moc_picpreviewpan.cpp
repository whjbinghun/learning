/****************************************************************************
** Meta object code from reading C++ file 'picpreviewpan.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VodPlayInterface/picpreviewpan.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'picpreviewpan.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PicPreviewPan_t {
    QByteArrayData data[8];
    char stringdata[94];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_PicPreviewPan_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_PicPreviewPan_t qt_meta_stringdata_PicPreviewPan = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 19),
QT_MOC_LITERAL(2, 34, 0),
QT_MOC_LITERAL(3, 35, 5),
QT_MOC_LITERAL(4, 41, 14),
QT_MOC_LITERAL(5, 56, 12),
QT_MOC_LITERAL(6, 69, 14),
QT_MOC_LITERAL(7, 84, 8)
    },
    "PicPreviewPan\0set_line_edit_value\0\0"
    "value\0slot_add_image\0slot_add_ptz\0"
    "PicPreviewInfo\0pic_info\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PicPreviewPan[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x0a,
       4,    0,   32,    2, 0x0a,
       5,    1,   33,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,

       0        // eod
};

void PicPreviewPan::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PicPreviewPan *_t = static_cast<PicPreviewPan *>(_o);
        switch (_id) {
        case 0: _t->set_line_edit_value((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->slot_add_image(); break;
        case 2: _t->slot_add_ptz((*reinterpret_cast< const PicPreviewInfo(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject PicPreviewPan::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_PicPreviewPan.data,
      qt_meta_data_PicPreviewPan,  qt_static_metacall, 0, 0}
};


const QMetaObject *PicPreviewPan::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PicPreviewPan::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PicPreviewPan.stringdata))
        return static_cast<void*>(const_cast< PicPreviewPan*>(this));
    return QFrame::qt_metacast(_clname);
}

int PicPreviewPan::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

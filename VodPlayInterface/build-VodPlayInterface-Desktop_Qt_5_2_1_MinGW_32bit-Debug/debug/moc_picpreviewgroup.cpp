/****************************************************************************
** Meta object code from reading C++ file 'picpreviewgroup.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VodPlayInterface/picpreviewgroup.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'picpreviewgroup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PicPreviewGroup_t {
    QByteArrayData data[8];
    char stringdata[89];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_PicPreviewGroup_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_PicPreviewGroup_t qt_meta_stringdata_PicPreviewGroup = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 9),
QT_MOC_LITERAL(2, 26, 0),
QT_MOC_LITERAL(3, 27, 14),
QT_MOC_LITERAL(4, 42, 7),
QT_MOC_LITERAL(5, 50, 14),
QT_MOC_LITERAL(6, 65, 16),
QT_MOC_LITERAL(7, 82, 5)
    },
    "PicPreviewGroup\0set_frame\0\0const QPointF*\0"
    "p_point\0slot_add_image\0set_v_edit_value\0"
    "value\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PicPreviewGroup[] = {

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
       5,    0,   32,    2, 0x0a,
       6,    1,   33,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,

       0        // eod
};

void PicPreviewGroup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PicPreviewGroup *_t = static_cast<PicPreviewGroup *>(_o);
        switch (_id) {
        case 0: _t->set_frame((*reinterpret_cast< const QPointF*(*)>(_a[1]))); break;
        case 1: _t->slot_add_image(); break;
        case 2: _t->set_v_edit_value((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject PicPreviewGroup::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_PicPreviewGroup.data,
      qt_meta_data_PicPreviewGroup,  qt_static_metacall, 0, 0}
};


const QMetaObject *PicPreviewGroup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PicPreviewGroup::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PicPreviewGroup.stringdata))
        return static_cast<void*>(const_cast< PicPreviewGroup*>(this));
    return QFrame::qt_metacast(_clname);
}

int PicPreviewGroup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

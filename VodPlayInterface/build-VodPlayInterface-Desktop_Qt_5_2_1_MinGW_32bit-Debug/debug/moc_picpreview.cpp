/****************************************************************************
** Meta object code from reading C++ file 'picpreview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VodPlayInterface/picpreview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'picpreview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PicPreview_t {
    QByteArrayData data[12];
    char stringdata[134];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_PicPreview_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_PicPreview_t qt_meta_stringdata_PicPreview = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 19),
QT_MOC_LITERAL(2, 31, 0),
QT_MOC_LITERAL(3, 32, 5),
QT_MOC_LITERAL(4, 38, 11),
QT_MOC_LITERAL(5, 50, 12),
QT_MOC_LITERAL(6, 63, 9),
QT_MOC_LITERAL(7, 73, 13),
QT_MOC_LITERAL(8, 87, 6),
QT_MOC_LITERAL(9, 94, 14),
QT_MOC_LITERAL(10, 109, 14),
QT_MOC_LITERAL(11, 124, 8)
    },
    "PicPreview\0set_line_edit_value\0\0value\0"
    "clear_image\0delete_image\0add_image\0"
    "str_file_path\0str_sn\0slot_add_image\0"
    "PicPreviewInfo\0pic_info\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PicPreview[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x0a,
       4,    0,   47,    2, 0x0a,
       5,    0,   48,    2, 0x0a,
       6,    2,   49,    2, 0x0a,
       9,    0,   54,    2, 0x0a,
       9,    1,   55,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Int, QMetaType::QString, QMetaType::QString,    7,    8,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,

       0        // eod
};

void PicPreview::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PicPreview *_t = static_cast<PicPreview *>(_o);
        switch (_id) {
        case 0: _t->set_line_edit_value((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->clear_image(); break;
        case 2: _t->delete_image(); break;
        case 3: { int _r = _t->add_image((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: _t->slot_add_image(); break;
        case 5: _t->slot_add_image((*reinterpret_cast< const PicPreviewInfo(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject PicPreview::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_PicPreview.data,
      qt_meta_data_PicPreview,  qt_static_metacall, 0, 0}
};


const QMetaObject *PicPreview::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PicPreview::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PicPreview.stringdata))
        return static_cast<void*>(const_cast< PicPreview*>(this));
    return QFrame::qt_metacast(_clname);
}

int PicPreview::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

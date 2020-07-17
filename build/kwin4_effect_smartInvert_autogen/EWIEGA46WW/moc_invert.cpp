/****************************************************************************
** Meta object code from reading C++ file 'invert.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "invert.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'invert.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_KWin__InvertEffect_t {
    QByteArrayData data[7];
    char stringdata0[94];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_KWin__InvertEffect_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_KWin__InvertEffect_t qt_meta_stringdata_KWin__InvertEffect = {
    {
QT_MOC_LITERAL(0, 0, 18), // "KWin::InvertEffect"
QT_MOC_LITERAL(1, 19, 21), // "toggleScreenInversion"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 12), // "toggleWindow"
QT_MOC_LITERAL(4, 55, 16), // "slotWindowClosed"
QT_MOC_LITERAL(5, 72, 19), // "KWin::EffectWindow*"
QT_MOC_LITERAL(6, 92, 1) // "w"

    },
    "KWin::InvertEffect\0toggleScreenInversion\0"
    "\0toggleWindow\0slotWindowClosed\0"
    "KWin::EffectWindow*\0w"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KWin__InvertEffect[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a /* Public */,
       3,    0,   30,    2, 0x0a /* Public */,
       4,    1,   31,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,

       0        // eod
};

void KWin::InvertEffect::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<InvertEffect *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->toggleScreenInversion(); break;
        case 1: _t->toggleWindow(); break;
        case 2: _t->slotWindowClosed((*reinterpret_cast< KWin::EffectWindow*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< KWin::EffectWindow* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject KWin::InvertEffect::staticMetaObject = { {
    QMetaObject::SuperData::link<Effect::staticMetaObject>(),
    qt_meta_stringdata_KWin__InvertEffect.data,
    qt_meta_data_KWin__InvertEffect,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *KWin::InvertEffect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KWin::InvertEffect::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_KWin__InvertEffect.stringdata0))
        return static_cast<void*>(this);
    return Effect::qt_metacast(_clname);
}

int KWin::InvertEffect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Effect::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

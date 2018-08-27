/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Map2d/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[17];
    char stringdata0[360];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 23), // "on_btn_open_cam_clicked"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 24), // "on_btn_open_list_clicked"
QT_MOC_LITERAL(4, 61, 20), // "on_btn_start_clicked"
QT_MOC_LITERAL(5, 82, 20), // "on_btn_pause_clicked"
QT_MOC_LITERAL(6, 103, 21), // "on_btn_resume_clicked"
QT_MOC_LITERAL(7, 125, 21), // "on_btn_finish_clicked"
QT_MOC_LITERAL(8, 147, 19), // "on_btn_save_clicked"
QT_MOC_LITERAL(9, 167, 28), // "on_btn_update_period_clicked"
QT_MOC_LITERAL(10, 196, 25), // "on_btn_open_video_clicked"
QT_MOC_LITERAL(11, 222, 31), // "on_slide_period_actionTriggered"
QT_MOC_LITERAL(12, 254, 6), // "action"
QT_MOC_LITERAL(13, 261, 12), // "updatePeriod"
QT_MOC_LITERAL(14, 274, 22), // "on_btn_preview_clicked"
QT_MOC_LITERAL(15, 297, 31), // "on_slide_offset_actionTriggered"
QT_MOC_LITERAL(16, 329, 30) // "on_slide_optim_actionTriggered"

    },
    "MainWindow\0on_btn_open_cam_clicked\0\0"
    "on_btn_open_list_clicked\0on_btn_start_clicked\0"
    "on_btn_pause_clicked\0on_btn_resume_clicked\0"
    "on_btn_finish_clicked\0on_btn_save_clicked\0"
    "on_btn_update_period_clicked\0"
    "on_btn_open_video_clicked\0"
    "on_slide_period_actionTriggered\0action\0"
    "updatePeriod\0on_btn_preview_clicked\0"
    "on_slide_offset_actionTriggered\0"
    "on_slide_optim_actionTriggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    0,   85,    2, 0x08 /* Private */,
       4,    0,   86,    2, 0x08 /* Private */,
       5,    0,   87,    2, 0x08 /* Private */,
       6,    0,   88,    2, 0x08 /* Private */,
       7,    0,   89,    2, 0x08 /* Private */,
       8,    0,   90,    2, 0x08 /* Private */,
       9,    0,   91,    2, 0x08 /* Private */,
      10,    0,   92,    2, 0x08 /* Private */,
      11,    1,   93,    2, 0x08 /* Private */,
      13,    0,   96,    2, 0x08 /* Private */,
      14,    0,   97,    2, 0x08 /* Private */,
      15,    1,   98,    2, 0x08 /* Private */,
      16,    1,  101,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_btn_open_cam_clicked(); break;
        case 1: _t->on_btn_open_list_clicked(); break;
        case 2: _t->on_btn_start_clicked(); break;
        case 3: _t->on_btn_pause_clicked(); break;
        case 4: _t->on_btn_resume_clicked(); break;
        case 5: _t->on_btn_finish_clicked(); break;
        case 6: _t->on_btn_save_clicked(); break;
        case 7: _t->on_btn_update_period_clicked(); break;
        case 8: _t->on_btn_open_video_clicked(); break;
        case 9: _t->on_slide_period_actionTriggered((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->updatePeriod(); break;
        case 11: _t->on_btn_preview_clicked(); break;
        case 12: _t->on_slide_offset_actionTriggered((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->on_slide_optim_actionTriggered((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

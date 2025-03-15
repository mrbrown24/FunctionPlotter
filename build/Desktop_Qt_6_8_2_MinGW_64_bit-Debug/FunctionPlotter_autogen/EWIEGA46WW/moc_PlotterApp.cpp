/****************************************************************************
** Meta object code from reading C++ file 'PlotterApp.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../PlotterApp.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PlotterApp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN17PlotterMainWindowE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN17PlotterMainWindowE = QtMocHelpers::stringData(
    "PlotterMainWindow",
    "onAddEquationClicked",
    "",
    "onRemoveEquationClicked",
    "onGeneratePlotClicked",
    "onClearPlotClicked",
    "onEquationSelectionChanged",
    "onEquationVisibilityChanged",
    "state",
    "onEquationColorChanged",
    "onLineWidthChanged",
    "width",
    "onEquationDoubleClicked",
    "QListWidgetItem*",
    "item",
    "onSavePlotAsImageClicked"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN17PlotterMainWindowE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   74,    2, 0x08,    1 /* Private */,
       3,    0,   75,    2, 0x08,    2 /* Private */,
       4,    0,   76,    2, 0x08,    3 /* Private */,
       5,    0,   77,    2, 0x08,    4 /* Private */,
       6,    0,   78,    2, 0x08,    5 /* Private */,
       7,    1,   79,    2, 0x08,    6 /* Private */,
       9,    0,   82,    2, 0x08,    8 /* Private */,
      10,    1,   83,    2, 0x08,    9 /* Private */,
      12,    1,   86,    2, 0x08,   11 /* Private */,
      15,    0,   89,    2, 0x08,   13 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   11,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject PlotterMainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_ZN17PlotterMainWindowE.offsetsAndSizes,
    qt_meta_data_ZN17PlotterMainWindowE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN17PlotterMainWindowE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<PlotterMainWindow, std::true_type>,
        // method 'onAddEquationClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onRemoveEquationClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onGeneratePlotClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onClearPlotClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onEquationSelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onEquationVisibilityChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onEquationColorChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onLineWidthChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'onEquationDoubleClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'onSavePlotAsImageClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void PlotterMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PlotterMainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->onAddEquationClicked(); break;
        case 1: _t->onRemoveEquationClicked(); break;
        case 2: _t->onGeneratePlotClicked(); break;
        case 3: _t->onClearPlotClicked(); break;
        case 4: _t->onEquationSelectionChanged(); break;
        case 5: _t->onEquationVisibilityChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->onEquationColorChanged(); break;
        case 7: _t->onLineWidthChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 8: _t->onEquationDoubleClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 9: _t->onSavePlotAsImageClicked(); break;
        default: ;
        }
    }
}

const QMetaObject *PlotterMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PlotterMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN17PlotterMainWindowE.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int PlotterMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP

/****************************************************************************
** Meta object code from reading C++ file 'qml_manager.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../controller/HEADERs/qml_manager.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qml_manager.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.1. It"
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
struct qt_meta_tag_ZN8emulator11qml_managerE_t {};
} // unnamed namespace

template <> constexpr inline auto emulator::qml_manager::qt_create_metaobjectdata<qt_meta_tag_ZN8emulator11qml_managerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "emulator::qml_manager",
        "scoreChanged",
        "",
        "activeGameChanged",
        "runningChanged",
        "pausedChanged",
        "gameOverChanged",
        "appStateChanged",
        "frameUpdated",
        "launchGame",
        "index",
        "returnToMenu",
        "sendKey",
        "key",
        "sendKeyByName",
        "keyName",
        "togglePause",
        "resetGame",
        "quitEmulator",
        "getScreenBufferSnapshot",
        "QVariantList",
        "onTick",
        "currentScore",
        "activeGameName",
        "activeGameDesc",
        "activeGameIndex",
        "isRunning",
        "isPaused",
        "isGameOver",
        "appState",
        "gameList",
        "frameText",
        "screenWidth",
        "screenHeight"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'scoreChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'activeGameChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'runningChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'pausedChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'gameOverChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'appStateChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'frameUpdated'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'launchGame'
        QtMocHelpers::SlotData<void(int)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 10 },
        }}),
        // Slot 'returnToMenu'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'sendKey'
        QtMocHelpers::SlotData<void(int)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 13 },
        }}),
        // Slot 'sendKeyByName'
        QtMocHelpers::SlotData<void(const QString &)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 15 },
        }}),
        // Slot 'togglePause'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'resetGame'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'quitEmulator'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'getScreenBufferSnapshot'
        QtMocHelpers::SlotData<QVariantList() const>(19, 2, QMC::AccessPublic, 0x80000000 | 20),
        // Slot 'onTick'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'currentScore'
        QtMocHelpers::PropertyData<int>(22, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'activeGameName'
        QtMocHelpers::PropertyData<QString>(23, QMetaType::QString, QMC::DefaultPropertyFlags, 1),
        // property 'activeGameDesc'
        QtMocHelpers::PropertyData<QString>(24, QMetaType::QString, QMC::DefaultPropertyFlags, 1),
        // property 'activeGameIndex'
        QtMocHelpers::PropertyData<int>(25, QMetaType::Int, QMC::DefaultPropertyFlags, 1),
        // property 'isRunning'
        QtMocHelpers::PropertyData<bool>(26, QMetaType::Bool, QMC::DefaultPropertyFlags, 2),
        // property 'isPaused'
        QtMocHelpers::PropertyData<bool>(27, QMetaType::Bool, QMC::DefaultPropertyFlags, 3),
        // property 'isGameOver'
        QtMocHelpers::PropertyData<bool>(28, QMetaType::Bool, QMC::DefaultPropertyFlags, 4),
        // property 'appState'
        QtMocHelpers::PropertyData<int>(29, QMetaType::Int, QMC::DefaultPropertyFlags, 5),
        // property 'gameList'
        QtMocHelpers::PropertyData<QVariantList>(30, 0x80000000 | 20, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'frameText'
        QtMocHelpers::PropertyData<QString>(31, QMetaType::QString, QMC::DefaultPropertyFlags, 6),
        // property 'screenWidth'
        QtMocHelpers::PropertyData<int>(32, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'screenHeight'
        QtMocHelpers::PropertyData<int>(33, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<qml_manager, qt_meta_tag_ZN8emulator11qml_managerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject emulator::qml_manager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8emulator11qml_managerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8emulator11qml_managerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN8emulator11qml_managerE_t>.metaTypes,
    nullptr
} };

void emulator::qml_manager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<qml_manager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->scoreChanged(); break;
        case 1: _t->activeGameChanged(); break;
        case 2: _t->runningChanged(); break;
        case 3: _t->pausedChanged(); break;
        case 4: _t->gameOverChanged(); break;
        case 5: _t->appStateChanged(); break;
        case 6: _t->frameUpdated(); break;
        case 7: _t->launchGame((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->returnToMenu(); break;
        case 9: _t->sendKey((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->sendKeyByName((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: _t->togglePause(); break;
        case 12: _t->resetGame(); break;
        case 13: _t->quitEmulator(); break;
        case 14: { QVariantList _r = _t->getScreenBufferSnapshot();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 15: _t->onTick(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (qml_manager::*)()>(_a, &qml_manager::scoreChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (qml_manager::*)()>(_a, &qml_manager::activeGameChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (qml_manager::*)()>(_a, &qml_manager::runningChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (qml_manager::*)()>(_a, &qml_manager::pausedChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (qml_manager::*)()>(_a, &qml_manager::gameOverChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (qml_manager::*)()>(_a, &qml_manager::appStateChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (qml_manager::*)()>(_a, &qml_manager::frameUpdated, 6))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->currentScore(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->activeGameName(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->activeGameDesc(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->activeGameIndex(); break;
        case 4: *reinterpret_cast<bool*>(_v) = _t->isRunning(); break;
        case 5: *reinterpret_cast<bool*>(_v) = _t->isPaused(); break;
        case 6: *reinterpret_cast<bool*>(_v) = _t->isGameOver(); break;
        case 7: *reinterpret_cast<int*>(_v) = _t->appState(); break;
        case 8: *reinterpret_cast<QVariantList*>(_v) = _t->gameList(); break;
        case 9: *reinterpret_cast<QString*>(_v) = _t->frameText(); break;
        case 10: *reinterpret_cast<int*>(_v) = _t->screenWidth(); break;
        case 11: *reinterpret_cast<int*>(_v) = _t->screenHeight(); break;
        default: break;
        }
    }
}

const QMetaObject *emulator::qml_manager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *emulator::qml_manager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8emulator11qml_managerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int emulator::qml_manager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void emulator::qml_manager::scoreChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void emulator::qml_manager::activeGameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void emulator::qml_manager::runningChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void emulator::qml_manager::pausedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void emulator::qml_manager::gameOverChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void emulator::qml_manager::appStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void emulator::qml_manager::frameUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP

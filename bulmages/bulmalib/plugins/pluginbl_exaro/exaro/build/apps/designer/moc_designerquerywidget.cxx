/****************************************************************************
** Meta object code from reading C++ file 'designerquerywidget.h'
**
** Created: Fri Jul 1 00:22:32 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../apps/designer/designerquerywidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'designerquerywidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Report__DesignerQueryWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      29,   28,   28,   28, 0x0a,
      42,   28,   28,   28, 0x0a,
      55,   28,   28,   28, 0x0a,
      66,   28,   28,   28, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Report__DesignerQueryWidget[] = {
    "Report::DesignerQueryWidget\0\0createItem()\0"
    "deleteItem()\0editItem()\0editName()\0"
};

const QMetaObject Report::DesignerQueryWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Report__DesignerQueryWidget,
      qt_meta_data_Report__DesignerQueryWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Report::DesignerQueryWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Report::DesignerQueryWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Report::DesignerQueryWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Report__DesignerQueryWidget))
        return static_cast<void*>(const_cast< DesignerQueryWidget*>(this));
    if (!strcmp(_clname, "Ui::designerQueryWidget"))
        return static_cast< Ui::designerQueryWidget*>(const_cast< DesignerQueryWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int Report::DesignerQueryWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: createItem(); break;
        case 1: deleteItem(); break;
        case 2: editItem(); break;
        case 3: editName(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
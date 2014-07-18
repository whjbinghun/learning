#ifndef PROPERTYBROWSERA_H
#define PROPERTYBROWSERA_H

#include <QWidget>
#include <QDate>
#include <QLocale>
#include <QApplication>
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qteditorfactory.h"
#include "qttreepropertybrowser.h"
#include "qtbuttonpropertybrowser.h"
#include "qtgroupboxpropertybrowser.h"

class PropertyDockWidget;

class PropertyBrowserA : public QWidget
{
    Q_OBJECT
public:
    explicit PropertyBrowserA(QWidget *parent = 0);
    ~PropertyBrowserA();
public:
    void init_button();
    void init_property_browser();
    void add_two_item();
    void resizeEvent( QResizeEvent *event );
    QtTreePropertyBrowser *get_tree_property_browser();
public slots:
    void slot_change_template_value( QtProperty *property , int value );
    void slot_change_color( QtProperty *property, const QColor &val );
    void slot_change_template( QtProperty *property, int val );
    void slot_bool_changed( QtProperty *, bool b_status );
private:
    QtVariantPropertyManager *mp_variant_manager;
    QtBoolPropertyManager *mp_bool_manager;
    QtIntPropertyManager *mp_int_manager;
    QtColorPropertyManager *mp_color_manager;
    QtEnumPropertyManager *mp_enum_manager;
    QtVariantEditorFactory *mp_variant_factory;
    QtEnumEditorFactory *mp_com_box_factory;
    QtColorEditorFactory *mp_color_factory;
    QtCheckBoxFactory *mp_check_box_factory;
    QtSpinBoxFactory *mp_spin_box_factory;
    QtTreePropertyBrowser *mp_variant_editor;
    bool mb_template_auto_status;
    QtProperty *mp_item_templdate;
    QtProperty *mp_max_template;
    QtProperty *mp_min_template;
    QtProperty *mp_item_ir_color;
    QtProperty *mp_item_steup;
    PropertyDockWidget *mp_dock_widget;
    QWidget *mp_parent;
};

#endif // PROPERTYBROWSERA_H

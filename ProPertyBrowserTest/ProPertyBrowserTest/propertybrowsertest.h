#ifndef PROPERTYBROWSERTEST_H
#define PROPERTYBROWSERTEST_H

#include <QMainWindow>
#include <QDate>
#include <QLocale>
#include <QApplication>
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qteditorfactory.h"
#include "qttreepropertybrowser.h"
#include "qtbuttonpropertybrowser.h"
#include "qtgroupboxpropertybrowser.h"
#include <QPushButton>

namespace Ui {
class ProPertyBrowserTest;
}

class ProPertyBrowserTest : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProPertyBrowserTest(QWidget *parent = 0);
    ~ProPertyBrowserTest();
public:
    void init_property_browser();
    void add_two_item();
    void resizeEvent( QResizeEvent *event );
public slots:
    void slot_change_template_value( QtProperty *property , int value );
    void slot_change_color( QtProperty *property, const QColor &val );
    void slot_change_template( QtProperty *property, int val );
    void slot_bool_changed( QtProperty *, bool b_status );
private:
    Ui::ProPertyBrowserTest *ui;
    QtVariantPropertyManager *mp_variant_manager;
    QtBoolPropertyManager *mp_bool_manager;
    QtIntPropertyManager *mp_int_manager;
    QtColorPropertyManager *mp_color_manager;
    QtEnumPropertyManager *mp_enum_manager;
    QtVariantEditorFactory *mp_variant_factory;
    QtTreePropertyBrowser *mp_variant_editor;
    QPushButton *mp_btn_template_data;
    bool mb_template_auto_status;
    QtProperty *mp_item_templdate;
    QtProperty *mp_max_template;
    QtProperty *mp_min_template;
    QtProperty *mp_item_ir_color;
    QtProperty *mp_item_steup;
    QtColorEditorFactory *mp_color_factory;
};

#endif // PROPERTYBROWSERTEST_H

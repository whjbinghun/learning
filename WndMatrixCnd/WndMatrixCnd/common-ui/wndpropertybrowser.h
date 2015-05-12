#ifndef WNDPROPERTYBROWSER_H
#define WNDPROPERTYBROWSER_H

#include <QWidget>
#include <QPushButton>
#include <QDate>
#include <QMutex>
#include <QLocale>
#include <QApplication>
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qteditorfactory.h"
#include "qttreepropertybrowser.h"
#include "qtbuttonpropertybrowser.h"
#include "qtgroupboxpropertybrowser.h"
#include <QSettings>

#ifndef DEFINE_YHANDLE
typedef void *YHANDLE;
#define DEFINE_YHANDLE
#endif

class ChartPoints;
class WndChart;

typedef struct _tagPropertyInfo{
    ChartPoints *p_chart;
    QtProperty *p_property_group;
    QtProperty *p_property_clr;
    _tagPropertyInfo()
        : p_chart(0)
        , p_property_group(0)
        , p_property_clr(0){}
} PropertyInfo;

class WndPropertyBrowser : public QWidget
{
    Q_OBJECT
public:
    explicit WndPropertyBrowser(WndChart *p_wnd_chart, QWidget *parent = 0);
    ~WndPropertyBrowser();

public:
    void resizeEvent( QResizeEvent *event );

    void init_wnd_ctrl( int n_group_number, QString str_sn, int n_index );

public:
    void init_button();

    void init_property();
    void create_property_setting();

    void create_property_ana( PropertyInfo &pro_info );
    void remove_property( QtProperty *p_item );
    void remove_clr_property( QtProperty *p_clr_item );

    ChartPoints *find_chart( QtProperty *p_clr_item );


    void add_property_ana( YHANDLE h_ana, int n_type, ChartPoints *p_chart );
    void delete_property_ana( YHANDLE h_ana, int n_type );
    void delete_property_ana( YHANDLE h_ana );

    // 设置存储间隔时间
    void set_storage_interval_time( int time);

signals:

public slots:
    void slot_show_chart();
    void slot_copy_temp();
    void slot_copy_chart();

    void slot_property_temp_change( QtProperty *property , int n_val );
    void slot_property_value_change( QtProperty *property, QColor clr );
    void slot_property_value_change( QtProperty *property, int n_val );
    void slot_property_value_change( QtProperty *, bool b_val );
private:
    void get_ana_chart_setting();
    void set_ana_chart_setting();

    void set_chart_interval_time( int n_val );

private:
    QtTreePropertyBrowser *mp_tree_property;
    QList< QtAbstractPropertyManager * > m_list_property_manager;
    QList< void * > m_list_property_factory;

    QtIntPropertyManager *mp_int_manager;
    QtProperty *mp_item_setting;
    QtProperty *mp_range_max;
    QtProperty *mp_range_min;
    QtProperty *mp_storage_interval_item;
    QtProperty *mp_temp_update_mode_item;
    QtProperty *mp_display_time_item;

    QMap< YHANDLE, QMap< int , PropertyInfo > > m_map_ana_property;
    QMutex m_mutex_ana;

//    QPushButton *mp_btn_show_chart;
//    QPushButton *mp_btn_copy_temp;
//    QPushButton *mp_btn_copy_chart;
    QWidget *mp_parent;
    WndChart *mp_wnd_chart;

    QtEnumPropertyManager *p_enum_manager;

    int mn_group_number;
    QString mstr_sn;
    int mn_index;


    int mn_temp_update_mode; // 温度更新方式, 0自动; 1手动
    float mf_min, mf_max;
    int mn_storage_time_interval_index; // 存储时间间隔
    unsigned int mnu_clear_period;

};

#endif // WNDPROPERTYBROWSER_H

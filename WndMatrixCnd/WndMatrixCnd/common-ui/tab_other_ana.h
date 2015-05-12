#ifndef TAB_OTHER_ANA_H
#define TAB_OTHER_ANA_H

#include <QWidget>
#include <QString>
#include <QTableWidget>
#include <QPushButton>
#include "dlgalarmcondition.h"
#include "datacommon.h"

class tab_other_ana : public QWidget
{
    Q_OBJECT
public:
    explicit tab_other_ana(QWidget *parent = 0);
    ~tab_other_ana();
    void insert_alarm_group( AlarmGroup *po_group );
    void init_table_widget();
    void clear_no_preset_alarm();  //清除所有报警条件
    void delete_object_btn( int n_row );
    void delete_alarm_group( int n_alarm_id );

    void alarm_group_to_string(AlarmGroup *p_alarm_group, QString &str_condition );
    void set_default_tab_width();
public:
    QTableWidget *mp_table_widget;
private:
    ANA_TYPE_E m_ana_type; //用来判断预置点分析还是实时分析
    QPushButton *mp_add_btn;
private slots:
    void slot_edit_tab();
    void slot_delete_tab();
    void slot_add_btn();
signals:
    void sig_delete_alarm_info( AlarmGroup *p_alarm_group );
    void sig_edit_alarm_info( AlarmGroup *p_alarm_group );
    void sig_add_alarm_info();
};

#endif // TAB_OTHER_ANA_H

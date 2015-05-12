#ifndef WNDALARMTRIGGERCND_H
#define WNDALARMTRIGGERCND_H

#include <QTableWidget>
#include "dlgalarmcondition.h"
#include "datacommon.h"
#include <QPushButton>
#include "wndcommon.h"

class WndAlarmTriggerCnd : public QTableWidget
{
    Q_OBJECT

public:
    explicit WndAlarmTriggerCnd(QWidget *parent = 0);
    ~WndAlarmTriggerCnd();

    void init_table_header();
    void insert_alarm_group( AlarmGroup *p_alarm_group , AnaType ana_type );
    /**
     * @brief clear_all_alarm_condition 清除所有报警条件
     */
    void clear_all_alarm_condition();
    /**
     * @brief clear_preset_alarm_condition 清除所有报警条件
     */
    void clear_preset_alarm_condition();
    void delete_object_btn( int n_row );
    void delete_alarm_group( int n_alarm_id );

    void split_string_to_alarm_group( QString s_alarm_condition, int n_alarm_id, QString &s_name, AlarmGroup *p_alarm_group ); //把报警条件信息QString转化为AlarmGroup类型
    void alarm_group_to_string( AlarmGroup *p_alarm_group, QString &str_condition );

    void update_cruise_list_cur_clr( int n_cur );
    void set_current_row( int n_current_row );
    int get_current_row();
    void resize_scoll( bool b_matrix_ana );
    void move_header();
public slots:
    void slot_cruise_list_clicked( QModelIndex index );

signals:
    void sig_delete_alarm_info( AlarmGroup *p_alarm_group );
    void sig_edit_alarm_info( AlarmGroup *p_alarm_group, QString str_ana_type );
    void sig_add_alarm_info();

public slots:

    void slot_edit_tab();
    void slot_delete_tab();
    void slot_add_btn();

private:
    void alarm_cond_init(QStringList s_alarm_cond_list, AlarmGroup *p_alarm_group);

private:
    int mn_current_alarm_cnd_row;
};

#endif // WNDALARMTRIGGERCND_H

﻿#ifndef TAB_ARRAY_ANA_H
#define TAB_ARRAY_ANA_H

#include <QWidget>
#include <QString>
#include <QListView>
#include <QTableWidget>
#include <QPushButton>
#include "dlgalarmcondition.h"

class tab_array_ana : public QWidget
{
    Q_OBJECT
public:
    explicit tab_array_ana(QWidget *parent = 0, ANA_TYPE_E ana_type = TAB_NONE_ANA );
    ~tab_array_ana();
    void insert_alarm_group( AlarmGroup *po_group );
    void clear_preset_alarm(); //清空预置点报警条件
    void split_string_to_alarm_group(QString s_alarm_condition, int n_alarm_id, QString &s_name, AlarmGroup *p_alarm_group ); //把报警条件信息QString转化为AlarmGroup类型
    void alarm_group_to_stringlist( AlarmGroup &alarm_group, QStringList &str_alarm_list );  //把AlarmGroup报警条件项转化为QStringList中
    void alarm_group_to_stringlist( AlarmGroup *p_alarm_group, QStringList &str_alarm_list ); //把AlarmGroup报警条件项转化为QStringList中
    void alarm_group_to_string(AlarmGroup *p_alarm_group,QString &str_condition );  //把AlarmGroup中的报警条件信息转化为QString类型
    void alarm_group_to_string(AlarmGroup &alarm_group,QString &str_condition );    //把AlarmGroup中的报警条件信息转化为QString类型
    void alarm_cond_init( QStringList s_alarm_cond_list, AlarmGroup *p_alarm_group );
    void delete_object_btn( int n_row );  //删除按钮对象
    void delete_alarm_group( int n_alarm_id );  //根据AlarmGroup中n_alarm_id删除报警条件项
    void init_table_widget();
    void set_default_tab_width();  //设置成默认宽度
public:
    QTableWidget *mp_table_widget;
private:
    ANA_TYPE_E m_ana_type;   //用来判断预置点分析还是实时分析
    QPushButton *mp_add_btn;
private slots:
    void slot_edit_tab();  //修改报警条件
    void slot_delete_tab();//增加报警条件
    void slot_add_btn();   //删除报警条件
signals:
    void sig_delete_alarm_info( AlarmGroup *p_alarm_group );
    void sig_edit_alarm_info( AlarmGroup *p_alarm_group );
    void sig_add_alarm_info();
};

#endif // TAB_ARRAY_ANA_H

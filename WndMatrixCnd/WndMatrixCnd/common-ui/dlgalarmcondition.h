#ifndef DLGALARMCONDITION_H
#define DLGALARMCONDITION_H

#include <QDialog>
#include "alarmcfg.h"
#include <QComboBox>

namespace Ui {
class DlgAlarmCondition;
}

typedef enum {
    TAB_NONE_ANA,
    TAB_REAL_ANA,
    TAB_PRESET_ANA
} ANA_TYPE_E;


class DlgAlarmCondition : public QDialog
{
    Q_OBJECT

public:
    explicit DlgAlarmCondition(QWidget *parent = 0);
    ~DlgAlarmCondition();
public:
    void set_ana_info( QStringList& s_list);
    void paintEvent( QPaintEvent *event );
    void set_default_ana_info(AlarmGroup *p_alarm_group); //默认选项
    void set_check_general(bool b_checked);
    void set_check_array(bool b_checked);
    void set_check_general_enable( bool b_enabled );
    void set_check_array_enable( bool b_enabled );
    void clear_combo_info();
    void set_matrix_bool( bool b_matrix );
    void set_combo_value( QObject *p_combo );
    void set_combo_oper_enable( bool b_enabled );
    //报警条件项初始化
    void init_alarm_group_value( QObject *p_combo_atype, QObject *p_combo_oper,
                                                    QObject *p_combo_btype, QObject *p_combo_compare, QObject *p_line_value );
    //清除一行
    void clear_row_info( QObject *p_combo_atype, QObject *p_combo_oper,
                                        QObject *p_combo_btype, QObject *p_combo_compare, QObject *p_line_value );
      void set_matrix_combo_enable( bool b_enabled );
      void set_general_combo_enable(bool b_enabled );
      void set_combo_enable( bool b_enabled );
      void set_combo_enable1( bool b_enabled );
      void init_combo_type( QComboBox *p_combo_type );
      void init_combo_oper( QComboBox *p_combo_oper );
      void init_combo_compare( QComboBox *p_combo_compare );
      void init_line_edit( QLineEdit *p_line_edit );
public slots:
      void slot_on_clear();
      void slot_on_ok();
      void slot_activated(QString str_oper);
      void slot_radio_btn();
signals:
      void sig_radio_btn( int n_radio );
public:
    AlarmGroup      m_alarm_group;
    bool mb_matrix_ana;  //是否为阵列分析
    Ui::DlgAlarmCondition *ui;
private:
    QStringList         ms_list;   // ana info;
    QPixmap m_pixmap_background;
};

#endif // DLGALARMCONDITION_H

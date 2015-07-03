#ifndef WNDALARMCNDLIST_H
#define WNDALARMCNDLIST_H

#include <QTreeWidget>

typedef struct _tagPresetAlarm{

}PresetAlarm;

typedef struct _tagMartixAlarm{

}MartixAlarm;


class WndAlarmCndList : public QTreeWidget
{
    Q_OBJECT
public:
    explicit WndAlarmCndList(QWidget *parent = 0);
    ~WndAlarmCndList();
public:
    //初始化报警列表
    void init_alarm_list();

    //增加预置点报警条件
    void add_preset_alarm_cnd();
    //增加矩阵报警条件
    void add_martix_alarm_cnd();
    //删除报警条件（清除所有报警条件)
    void clear_all_alarm_cnd();
    //修改报警条件

    //选择单条报警条件
    void select_simple_alarm_cnd();
    //选择预置点报警条件
    void select_preset_alarm_cnd();
    //选择矩阵报警条件
    void select_martix_alarm_cnd();
    //选择全部报警条件
    void select_all_alarm_cnd();

    //取消单条报警条件选择
    void cancel_simple_alarm_cnd();
    //取消预置点报警条件选择
    void cancel_preset_alarm_cnd();
    //取消矩阵报警条件选择
    void cancel_martix_alarm_cnd();
    //取消全部报警条件选择
    void cancel_all_alarm_cnd();

signals:

public slots:

private:
    //预置点报警列表

    //矩阵报警列表

};

#endif // WNDALARMCNDLIST_H

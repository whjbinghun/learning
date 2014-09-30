#ifndef SWITCHWINNUM_H
#define SWITCHWINNUM_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QModelIndex>
#include "groupnumview.h"


namespace Ui {
class SwitchWinNum;
}

class SwitchWinNum : public QMainWindow
{
    Q_OBJECT

public:
    explicit SwitchWinNum(QWidget *parent = 0);
    ~SwitchWinNum();
public:
    void resizeEvent( QResizeEvent *event );
    void init_group_list();
    void init_connect();
    void move_widget();
    void show_frame();
    void widget_attribute();
    void init_list_view();
    void mouse_release( const QPointF &pt, int nx, int ny , QString str_group_name );

    void add_group_name(int n_num, QString str_group_name);
    void del_group_name(int n_num);
    void change_group_name(int n_num, QString str_group_name);
    void init_lab_move();
public slots:
    void slot_show_two_widget();
    void slot_show_six_widget();
private:
    Ui::SwitchWinNum *ui;
    int mn_widget_show_num;//窗口显示个数
    bool mb_mouse_press;
    QMap< int, QString> m_map_group_name;//int:窗口号，Qstring：组名(组名不能重复)
};

#endif // SWITCHWINNUM_H

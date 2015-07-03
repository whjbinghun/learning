#ifndef WNDMATRIXALARMLIST_H
#define WNDMATRIXALARMLIST_H

#include <QTableWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QEvent>

//预置点信息
typedef struct _tagMartixInfo {
    QString str_martix_name;
    QColor clr_martix;
    QString str_martix_alarm_cnd;//报警条件

    _tagMartixInfo()
        :str_martix_name( "" )
        ,clr_martix( Qt::blue )
        ,str_martix_alarm_cnd( "" ){}
} MartixInfo;

class WndMatrixAlarmList : public QTableWidget
{
    Q_OBJECT
public:
    explicit WndMatrixAlarmList(QWidget *parent = 0);
    ~WndMatrixAlarmList();
public:
    void init_ctrl();
    void init_connect();
    void init_lab( QLabel *p_lab );

    void update_alarm_list_width();

    void resizeEvent( QResizeEvent *event );
    void mouseMoveEvent( QMouseEvent *event );//当鼠标移动到报警条件时，显示lab，并置顶，放在鼠标尾
    //添加矩阵列表
    void add_martix_tab_list( MartixInfo &martix_info );
    void add_martix_tab( MartixInfo &martix_info );
    //删除矩阵列表
    void del_martix_tab();
    //修改矩阵名

    //添加报警条件或者编辑报警条件

    //删除报警条件

    void set_martix_list_cur( int n_row );

    void update_martix_alarm_list();
    void clear_martix_alarm_tab();

signals:

public slots:
    void slot_tab_martix_double_clicked( QTableWidgetItem *p_item );
    void slot_tab_martix_item_change( QTableWidgetItem *current, QTableWidgetItem *previous );

    void slot_color_dialog();
    void slot_martix_ana_info( QString str_matrix_name, QColor clr_matrix );
private:
    int mn_cur_martix_list_row;//选中的当前行
    int mn_eq_width;
    QList<MartixInfo*> m_list_martix_alarm;

    QLabel *mp_lab_show_alarm_cnd;
};

#endif // WNDMATRIXALARMLIST_H

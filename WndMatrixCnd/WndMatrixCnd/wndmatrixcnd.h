#ifndef WNDMATRIXCND_H
#define WNDMATRIXCND_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QTableWidgetItem>
#include <QLabel>
#include "wndmatrixalarmlist.h"
#include "dlgalarmcondition.h"
#include "netcommon.h"

namespace Ui {
class WndMatrixCnd;
}

class WndMatrixCnd : public QMainWindow
{
    Q_OBJECT

public:
    explicit WndMatrixCnd(QWidget *parent = 0);
    ~WndMatrixCnd();
public:
    void init_ctrl();
    void init_connect();

    void mouseMoveEvent( QMouseEvent *event );

    void set_anaitem_to_stringlist( AnaItem &ana_item, QStringList &s_alarm_list );
    void change_id_to_serverid( QString str_id, QString &str_serverid );
    void replaceidstr( int indexnum, QString &str_id );
    void get_matrix_color_name_by_ana_id( int n_ana_id, QString &str_out_color_name );
public slots:
    void slot_add_martix();
    void slot_del_martix();
    void slot_add_martix_cnd();
    void slot_del_martix_cnd();

private:
    Ui::WndMatrixCnd *ui;
    DlgAlarmCondition *mp_alarm_cond_dlg;

    QMap< int, int > m_map_id;  // 窗口id, 服务返回的分析ID,一个窗口id对应一个分析ID
    QMap< int, QString> m_array_id;//
};

#endif // WNDMATRIXCND_H

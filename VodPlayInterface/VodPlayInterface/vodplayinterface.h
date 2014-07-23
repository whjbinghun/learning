#ifndef VODPLAYINTERFACE_H
#define VODPLAYINTERFACE_H

#include <QMainWindow>
#include "titlebar.h"
#include <QPoint>
#include <QRect>
#include <QMenu>
#include <QTcpSocket>
#include <QMutex>

#include "netcontrol.h"


namespace Ui {
class VodPlayInterface;
}

class VodPlayInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit VodPlayInterface( QWidget *parent = 0 );
    ~VodPlayInterface();

    // 枚举
    enum Direction// 即控制的按键
    {
        eTop            = Qt::Key_W,
        eBottom         = Qt::Key_S,
        eRight          = Qt::Key_D,
        eLeft           = Qt::Key_A,
        eTopRight       = Qt::Key_E,
        eBottomLeft     = Qt::Key_Z,
        eRightBottom    = Qt::Key_X,
        eLeftTop        = Qt::Key_Q,
        eNone           = Qt::Key_N
    } enum_Direction;

public:
    void init_title_bar();
    void init_mainwindow();
    void init_window_connect();
    void init_mouse_tracking();
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void mouseDoubleClickEvent( QMouseEvent *event );
    void resizeEvent( QResizeEvent *event );
    void SetCursorStyle( Direction direction );
    void SetDrayMove( int nx_global, int ny_global, Direction direction );
    Direction PointValid( double x, double y );
    void paintEvent( QPaintEvent *event );


    void init_timer();
    void start_heartbeat();

    void init_net_timer();
    void uninit_net_timer();

signals:
    void close_window();
    void min_window();
    void max_window();
    void sig_display_monitor_interface();
    void sig_display_config_interface();
    void sig_display_analyse_interface();
    void sig_display_back_interface();

public slots:
    void slot_display_config_interface();
    void slot_display_monitor_interface();
    void slot_display_analyse_interface();
    void slot_display_back_interface();
    void show_contextmenu( const QPoint &pos );
    void slot_show_max_size();
    void send_update_xml();
    void send_heart_tag();

public:
    Ui::VodPlayInterface *ui;

private slots:

private: //窗口
    TitleBar * mp_title_bar;
    QPoint m_pt_press_global;
    bool mb_left_btn_press;
    bool mb_max_win;
    bool mb_max_size;//窗口最大化和还原
    QRect m_rect_restore_window;
    int mn_stackwindow_height;
    int mn_stackwindow_width;
    QMenu *mp_cmenu;
    Direction me_direction;

private:
    QTimer *mp_update_timer;
    QTimer *mp_timer_heart_tag;
    QTimer *mp_timer_update_xml;

    NetControl *mp_netcontrol;
};

#endif // VODPLAYINTERFACE_H

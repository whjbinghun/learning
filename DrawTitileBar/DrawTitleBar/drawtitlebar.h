#ifndef DRAWTITLEBAR_H
#define DRAWTITLEBAR_H

#include <QMainWindow>
#include <QVBoxLayout>
#include "titlebar.h"
#include <QPoint>
#include <QMouseEvent>
#include <QRect>




namespace Ui {
class DrawTitleBar;
}

class DrawTitleBar : public QMainWindow
{
    Q_OBJECT

public:
    explicit DrawTitleBar( QWidget *parent = 0 );
    ~DrawTitleBar();

public:

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

    void member_init();
    void bar_init();
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void mouseDoubleClickEvent( QMouseEvent *event );
    void SetCursorStyle( Direction direction );
    void SetDrayMove( int nx_global, int ny_global, Direction direction );
    void SetDrayMove( QMouseEvent *event, int nx_global, int ny_global, Direction direction);
    void resizeEvent( QResizeEvent *event );
    void paintEvent(QPaintEvent *event);
    bool enterBtn(QPoint pp, QToolButton *btn);
    Direction PointValid(int x, int y);
    void init_mini_window(QRect rectWindow);
signals:
    void close_window();

private:

    Ui::DrawTitleBar *ui;
    QVBoxLayout *mp_main_layout;
    TitleBar *mp_title_bar;
    QPoint m_pt_press_global;
    bool mb_left_btn_press;
    bool mb_max_win;
    QRect m_rect_restore_window;
    Direction m_eDirection;
    QLabel * mp_mouse_pos_label;


};

#endif // DRAWTITLEBAR_H

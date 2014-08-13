#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>


//枚举，按钮状态
enum eBtnMoustState{
     eBtnStateNone,//无效
     eBtnStateDefault,//默认值(如按钮初始显示)
     eBtnStateHover,//鼠标移到按钮上状态
     eBtnStatePress//鼠标按下按钮时状态
};


class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar( QWidget *parent = 0 );

public:
    void create_widget();
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );

    void set_btn_icon( QToolButton *pBtn, eBtnMoustState state, bool bInit );
    const QString get_btn_image_path( QToolButton *pBtn, bool bInit );
    void resizeEvent( QResizeEvent *event );
    void paintEvent( QPaintEvent *event );
signals:
    void signal_min();
    void signal_close();

public slots:
    void slot_btnpress();
    void slot_btnclick();
private:
    bool mb_left_button_press;
    QPoint m_pt_press;
    QPoint m_pt_move;
    QLabel *mp_label_icon;
    QLabel *mp_label_title;
    QToolButton *mp_btn_min;
    QToolButton *mp_btn_close;
};

#endif // TITLEBAR_H

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
    void CreateWidget();
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );

    void SetBtnIcon( QToolButton *pBtn, eBtnMoustState state, bool bInit );
    const QString GetBtnImagePath( QToolButton *pBtn, bool bInit );
    void resizeEvent( QResizeEvent *event );
    void paintEvent( QPaintEvent *event );
    void SetWidgetStyle();
    void CreateLayout();
signals:
    void signal_min();
    void signal_max_restore();
    void signal_close();
    void showSkin();
    void showMin();
    void showMax();
    void closeWidget();

public slots:
    void slot_btnpress();
    void slot_btnclick();

private:
    bool mb_left_button_press;
    QPoint m_pt_press;
    QPoint m_pt_move;
    QLabel *mp_label_icon;
    QLabel *mp_label_title;
    QLabel *mp_label_version;
    QToolButton *mp_btn_skin;
    QToolButton *mp_btn_menu;
    QToolButton *mp_btn_min;
    QToolButton *mp_btn_max;
    QToolButton *mp_btn_close;


    QPainter *mp_test_painter;
    QHBoxLayout *mp_layout;




};

#endif // TITLEBAR_H

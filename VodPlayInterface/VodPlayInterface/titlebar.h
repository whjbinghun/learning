#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QMouseEvent>
#include <QPoint>
#include <QPushButton>

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
    explicit TitleBar(QWidget *parent = 0);

public:
    void CreateWidget();
    void SetBtnIcon( QToolButton *pBtn, eBtnMoustState state, bool bInit );
    const QString GetBtnImagePath( QToolButton *pBtn, bool bInit );
    void resizeEvent( QResizeEvent *event );
    bool is_click_in_btn( int x, int y );

    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
signals:
    void signal_close();
public slots:
    void slot_btnpress();
    void slot_btnclick();
private:
    QLabel *mp_label_icon;
    QLabel *mp_label_title;
    QLabel *mp_label_version;
    QToolButton *mp_btn_close;
    QToolButton *mp_btn_min;
    QToolButton *mp_btn_max;
    QPoint m_pt_press;
    QPoint m_pt_move;
    bool mb_left_button_press;
public:
    QPushButton *mp_btn_monitor_interface;
    QPushButton *mp_btn_config_interface;
    QPushButton *mp_btn_analyse_interface;
    QPushButton *mp_btn_backplay_interface;

};

#endif // TITLEBAR_H

#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QToolButton>
#include <QPushButton>

class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = 0);

    void resizeEvent( QResizeEvent *event );
    void paintEvent( QPaintEvent *event );
    bool eventFilter( QObject *obj, QEvent *event );

    enum BtnMoustState{
         eBtnStateNone,//无效
         eBtnStateDefault,//默认值(如按钮初始显示)
         eBtnStateHover,//鼠标移到按钮上状态
         eBtnStatePress//鼠标按下按钮时状态
    };

    void set_btn_icon( QToolButton *p_btn, BtnMoustState state );
    QString get_btn_image_path( QToolButton *p_btn );

signals:
    void sig_close_window();
private slots:
    void slot_close_window();
private:
    QString m_str_title;
    QFont m_font_title;

    QPixmap m_pixmap_backgroud;
    QPixmap m_pixmap_logo;

private:
    QToolButton *mp_btn_close;
    QToolButton *mp_btn_min;

    QList< QPushButton * > m_list_btn;
};

#endif // TITLEBAR_H

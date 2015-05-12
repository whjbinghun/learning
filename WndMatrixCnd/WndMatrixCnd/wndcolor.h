#ifndef WNDCOLOR_H
#define WNDCOLOR_H

#include <QWidget>
#include <QColor>
#include <QMouseEvent>

class WndColor : public QWidget
{
    Q_OBJECT
public:
    explicit WndColor(QWidget *parent = 0);
    ~WndColor();
public:
    //设置颜色
    void set_color( QColor clr );
    //获取颜色
    QColor get_color();

    void set_row( int n_row );
    int get_row();
    //画颜色
    void paintEvent( QPaintEvent *event );
    void mousePressEvent( QMouseEvent *event );
signals:
    //鼠标左击，发送跳出颜色对话框的信号
    void sig_color_dialog();
public slots:

private:
    QColor m_clr;
    int mn_row;//该控件在列表中的行号
};

#endif // WNDCOLOR_H

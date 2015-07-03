#ifndef WNDWIDGETTWO_H
#define WNDWIDGETTWO_H

#include <QWidget>
#include <QMouseEvent>

class WndEventTest;

class WndWidgetTwo : public QWidget
{
    Q_OBJECT
public:
    explicit WndWidgetTwo(QWidget *parent = 0);

    void paintEvent( QPaintEvent *p_event );
    void mousePressEvent(QMouseEvent *p_event);
    //virtual bool event( QEvent *p_event );
public:
    void init_ctrl( WndEventTest *p_wnd_event );
signals:

public slots:
private:
    WndEventTest *mp_wnd_event;
};

#endif // WNDWIDGETTWO_H

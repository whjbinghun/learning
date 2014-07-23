#ifndef WNDIRDISPLAY_H
#define WNDIRDISPLAY_H

#include <QFrame>

class NetControl;

class WndIRDisplay : public QFrame
{
    Q_OBJECT
public:
    explicit WndIRDisplay(QWidget *parent = 0);

public:
    void set_net_ctrl( NetControl *p_net_ctrl ) { mp_net_control = p_net_ctrl; }

public:
    void paintEvent( QPaintEvent *event );

signals:

public slots:


private:
    NetControl *mp_net_control;
};

#endif // WNDIRDISPLAY_H

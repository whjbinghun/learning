#ifndef WNDPREVIEW_H
#define WNDPREVIEW_H

#include <QFrame>

class NetControl;

class WndPreview : public QFrame
{
    Q_OBJECT
public:
    explicit WndPreview(QWidget *parent = 0);

public:
    void set_net_ctrl( NetControl *p_net_ctrl ) { mp_net_control = p_net_ctrl; }
    void set_group_number( int n_num ) { mn_group_number = n_num; }

public:
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void mouseDoubleClickEvent( QMouseEvent *event );
    void resizeEvent( QResizeEvent *event );
    void paintEvent( QPaintEvent *event );

signals:

public slots:

private:
    int mn_group_number;
    NetControl *mp_net_control;
};

#endif // WNDPREVIEW_H

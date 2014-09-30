#ifndef WNDPANRAIL_H
#define WNDPANRAIL_H

#include <QWidget>

class WndPanRail : public QWidget
{
    Q_OBJECT
public:
    explicit WndPanRail(QWidget *parent = 0);
    ~WndPanRail();
public:
    void resizeEvent( QResizeEvent *event );
    void move_pan_btn();
private:
    QWidget *mp_pan_rail_widget;
    QWidget *mp_pan_widget;
    bool mb_rail_hidden;
    bool mb_pan_hidden;
    bool mb_rail_left_right_ver;
    bool mb_rail_lefttop_rightbottom_hor;
};

#endif // WNDPANRAIL_H

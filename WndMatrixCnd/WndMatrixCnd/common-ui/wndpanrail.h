#ifndef WNDPANRAIL_H
#define WNDPANRAIL_H

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>

class WndPanRail : public QWidget
{
    Q_OBJECT
public:
    explicit WndPanRail(QWidget *parent = 0);
    ~WndPanRail();

public:
    void resizeEvent( QResizeEvent *event );
    void paintEvent(QPaintEvent *event);

public:
    void init_widget();

    void set_group_num( int n_num ) { mn_group_num = n_num; }
    void set_rail_exist( bool b_rail_hidden, bool b_rail_ver_hor , bool b_rail_lefttop_or_rightbottom );
    void set_pan_exit( bool b_pan_hidden );
    void set_box_exit( bool b_box_hidden );
    void set_widget_transparency( float f_level );

    void update_ctrl();
    void update_ctrl_show();
    void update_ctrl_pos();

    void set_pan_effective( bool b_pan_effect );
    void set_rail_effective( bool b_rail_effect );

    void show_rail_type( int n_class );


    void set_rail_visiable( bool b_visiable );
private:
    void move_ptz_ctrl( QRect rct );
    void move_set_ctrl( QRect rct );
    void move_rail_ctrl( QRect rct );

    void show_hor_rail_ctrl( bool b_show );
    void show_ver_rail_ctrl( bool b_show );
    void show_ptz_ctrl( bool b_show );
    void show_box_ctrl( bool b_show );


public:
    QPushButton *mp_btn_left;
    QPushButton *mp_btn_lefttop;
    QPushButton *mp_btn_top;
    QPushButton *mp_btn_righttop;
    QPushButton *mp_btn_right;
    QPushButton *mp_btn_rightbottom;
    QPushButton *mp_btn_bottom;
    QPushButton *mp_btn_leftbottom;

    QSlider *mp_slider_ver;//垂直
    QPushButton *mp_btn_ver_top;
    QPushButton *mp_btn_ver_bottom;

    QSlider *mp_slider_hor;//水平
    QPushButton *mp_btn_hor_left;
    QPushButton *mp_btn_hor_right;

    QGroupBox *mp_box_set_pre;
    QLabel *mp_lab_rail_pre;
    QLineEdit *mp_edit_rail_pre;
    QPushButton *mp_btn_set_rail_pre;

    QLabel *mp_lab_pan_pre;
    QLineEdit *mp_edit_pan_pre;
    QPushButton *mp_btn_set_pan_pre;

    int mn_group_num;

private:
    bool mb_pan_hidden;
    bool mb_rail_hidden;
    bool mb_rail_lefttop_rightbottom;//true:lefttop ,false:rightbottom
    bool mb_rail_ver_hor;//True：ver， FALSE：hor
    bool mb_box_hidden;
};

#endif // WNDPANRAIL_H

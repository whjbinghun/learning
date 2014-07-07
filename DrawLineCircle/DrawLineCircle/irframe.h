#ifndef IRFRAME_H
#define IRFRAME_H

#include <QFrame>
#include <QLabel>
#include <QList>
#include <QMouseEvent>
#include <QSlider>
#include "irslider.h"
#include <QMap>

typedef struct {
    int n_sign_id;
    QString type_name;
    QPoint point_start;
    QPoint point_end;
}AnaInfo;

enum AnaMove{
    move_null = 0L,
    move_self,
    move_left,
    move_lefttop,
    move_top,
    move_righttop,
    move_right,
    move_rightbottom,
    move_bottom,
    move_leftbottom
};

class IrFrame : public QFrame
{
    Q_OBJECT
public:
    explicit IrFrame(QWidget *parent = 0);
    ~IrFrame();
public:
    enum enum_press_status{
        none_press_status = 0,
        point_status = 1,
        line_status = 2,
        rect_status = 3,
        circle_status = 4
    } me_press_status;
    enum enum_draw_status{
        none_draw_status = 0,
        draw_point_status = 1,
        draw_line_status = 2,
        draw_rect_status = 3,
        draw_circle_status = 4
    } me_draw_status;

    void paintEvent( QPaintEvent *event );
    void resizeEvent( QResizeEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void init_ir_widget();
    void set_ir_slider( IrSlider *p_ir_slider );

    void press_status_shape( QPoint pt );
    void draw_add_shape( QPoint pt );

    bool pt_in_ana( const QPoint &pt, AnaMove &ana_move, AnaInfo &ana_info );
    AnaMove pt_in_ana_point(const QPoint &pt, const AnaInfo &ana_info);
    AnaMove pt_in_ana_line( const QPoint &pt, const AnaInfo &ana_info );
    AnaMove pt_in_ana_rect( const QPoint &pt, const AnaInfo &ana_info );
    AnaMove pt_in_ana_circle( const QPoint &pt, const AnaInfo &ana_info );
    void get_pt_ana_shape( AnaInfo &ana_info, const QPoint &pt );
    void pt_move_ana_point( const QPoint &pt );
    void pt_move_ana_line( const QPoint &pt );
    void pt_move_ana_rect( const QPoint &pt );
    void pt_move_ana_circle( const QPoint &pt );
    void pt_move_self_boundary( QPoint &pt_start, QPoint &pt_end );
    void pt_circle_move_self_boundary( QPoint &pt_circle_center, QPoint &pt_end );

    void pt_judge_area( const QPoint &pt, QPoint &pt_area );
    void normalized_pt( QPoint &pt1, QPoint &pt2 );
    void set_cursor_pos( );
    void start_area( QPoint &pt_start, int &n_r );
    void shape_append_list( QPoint pt );
    void shape_move_or_change_size( QPoint pt );
    void judge_area( QPoint &pt_area );
public:
    void set_org_sz( int width, int height ) ;
signals:

public slots:
    void slot_slider_value();
    void slot_value_change( int n_pos );
private:
    QLabel *mp_transparent_ir_label;
    QList<QString> m_list;
    QPoint m_pt_start;
    QPoint m_pt_middle;
    IrSlider *mp_ir_slider;
    int mn_ir_slider_value;
    QList<AnaInfo> m_list_anainfo;
    bool mb_move_type;
    bool mb_change_size;

    QPoint m_press_pt;
    QPoint m_release_pt;
    QString ms_type_name;
    int mn_sign_id;
    AnaMove me_ana_move;
    QPoint m_pt_move_start;
    QPoint m_pt_move_end;
private:
    //窗口的原始size
    int mn_org_width;
    int mn_org_height;
    //当前窗口的size与原始size的比例
    float mf_sz_width;
    float mf_sz_height;
};

#endif // IRFRAME_H

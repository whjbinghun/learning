#ifndef WNDCHART_H
#define WNDCHART_H

#include <QWidget>
#include <QPainter>
#include <QList>
#include <QVector>
#include <QMap>
#include <QToolButton>

#define HEIGHT_TIME ( 0 )
#define HEIGHT_TITLE 30
#define SPACE_LEN 8

#define LEN_MARK 7
#define LEN_MARK_MAJOR 3

#define SPACE_AXIS_TO_TXT 11
#define SPACE_TXT_TO_BRODER 14
#define HEIGHT_AXIS_Y (SPACE_AXIS_TO_TXT+SPACE_TXT_TO_BRODER)
#define WIDTH_AXIS_X HEIGHT_AXIS_Y

#define WIDTH_AXIS_scroll 10

#define COLOR_AXIS ( QColor( 76, 92, 43 ) )

typedef struct _tagMarkInfo{
    int n_pix;
    float f_pos;
    QString str_temp;
} MarkInfo;

class WndChart : public QWidget
{
    Q_OBJECT
public:
    explicit WndChart(QWidget *parent = 0);
    ~WndChart();
public:
    virtual void paintEvent( QPaintEvent *event );
    virtual void mousePressEvent( QMouseEvent *event );
    virtual void mouseMoveEvent( QMouseEvent *event );
    virtual void mouseReleaseEvent( QMouseEvent *event );
    virtual void mouseDoubleClickEvent( QMouseEvent *event );
    virtual void resizeEvent( QResizeEvent *event );
    virtual void wheelEvent( QWheelEvent *event );

public:
    void cal_axis_x_span();
    void cal_axis_x_mark_info();

    void cal_axis_y_span();
    void cal_axis_y_mark_info();

    void cal_axis_y_scroll();

    void get_axis_x_mark_temp_and_pix( float &f_min_temp, int &n_min_pix );
    void get_axis_y_mark_pos_and_pix( float &f_min_pos, int &n_min_pix );

    void get_axis_x_range( float &f_min, float &f_max );

    void set_axis_x_auto_range( bool b_auto ) { mb_axisx_auto_range = b_auto;  }
    void set_axis_x_min( float f_min ){ mf_axisx_min = f_min; }
    void set_axis_x_max( float f_max ){ mf_axisx_max = f_max; }

    float get_axis_x_min() { return mf_axisx_min; }
    float get_axis_x_max() { return mf_axisx_max; }

    void set_axis_y_show_all(){ mb_axisy_scroll = false; }
    void copy_chart();

    bool pt_in_chart( const QPoint &pt );
    bool pt_in_scroll( const QPoint &pt );
    void move_chart( const QPoint &pt );
    void move_scroll( const QPoint &pt );
    void move_mark_line( const QPoint &pt );
    void move_scroll( const float &f_offset_left, const float &f_offset_right );

    void normalize_scroll( float &f_min, float &f_max );
    void update_axis_y();
    void update_axis_x();

protected:
    QToolButton* mp_show_all_trend;
    QToolButton* mp_copy_trend;
    QToolButton* mp_copy_temp;
    QToolButton* mp_select_color;

public:
    void draw_title( QPainter &draw );
    void draw_axis( QPainter &draw );
    void draw_chart( QPainter &draw );
    void draw_time( QPainter &draw );

    void draw_axis_x( QPainter &draw, const QRect &rct );
    void draw_axis_y( QPainter &draw, const QRect &rct );
    void draw_axis_y_scroll( QPainter &draw, const QRect &rct );

    void draw_axis_backgroud( QPainter &draw, const QRect &rct  );

    virtual void draw_chart_line( QPainter &draw, const QRect &rct  );
    virtual void show_tool_tip( const QPoint &pt, int n_pos_y );
    virtual void draw_chart_time( QPainter &draw, const QRect &rct );

    QFont m_fnt_axis;

    float mf_axisx_min, mf_axisx_max;                            //.纵轴的范围
    bool mb_axisx_auto_range;

    float mf_axisy_min, mf_axisy_max;                            //.横轴的范围
    float mf_axisy_min_scroll, mf_axisy_max_scroll;       //.横轴滑块的范围
    bool mb_axisy_scroll;                                                 //.横轴滑块是否存在
    int mn_axisy_scroll_left;                                            //.横轴滑块左边
    int mn_axisy_scroll_len;                                             //.横轴滑块长度

    QString mstr_title;

    float mf_axisx_mark_span_temp;                              //.纵轴单个间隔范围
    int mn_axisx_mark_span_pixel;                                 //.纵轴单个间隔所占像素点
    int mn_axisx_mark_text_width;                                 //.纵轴的文本所占宽度
    QList< MarkInfo > m_list_axisx_mark;                     //.纵轴所有间隔的位置及文本

    float mf_axisy_mark_span_pos;
    float mf_axisy_mark_span_pixel;
    QList< MarkInfo > m_list_axisy_mark;

    enum {
        mouse_down_null = 0,
        mouse_down_chart,
        mouse_down_scroll
    };

    int mn_mouse_down;
    QPoint m_pt_mouse_down;

    int mn_mark_line_y;

signals:

private slots:
private:

};

#endif // WNDCHART_H

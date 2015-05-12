#ifndef WNDPALETTE_H
#define WNDPALETTE_H

#include <QWidget>
#include <QImage>
#include <QVector>
#include <QMutex>
#include "../../public/ir-libs/YFIRImageEx1.h"

// #define COLOR_DEPTH 163
//enum ColorIndex
//{
//    clrNone=-1,
//    clrIron=1,					///< 铁红
//    clrRainbow=2,				///< 彩虹
//    clrFeather=3,				///< 医疗
//    clrGray=4,					///< 灰度
//    clrIronReverse=5,		///< 反转铁红
//    clrRainbowReverse=6,///< 反转彩虹
//    clrFeatherReverse=7,	///< 反转医疗
//    clrGrayReverse=8,		///< 反转灰度
//};

//enum YFAdjustType{
//    AdjustAuto=0,			///< 自动调节
//    AdjustManual=1,		///< 手动调节
//    AdjustUseFile=2,	///< 按照文件录制时调节
//};


class WndPalette : public QWidget
{
    Q_OBJECT
public:
    explicit WndPalette(QWidget *parent = 0);
    ~WndPalette();

public:
    void set_move_update( bool b_update ) { mb_move_update = b_update; }
    void set_cursor_wnd( QWidget *p_wnd ){ mp_cursor_wnd = p_wnd; }
    void init_palette( ColorIndex clr, bool b_auto
                       , float f_min, float f_max, float f_level, float f_level_span );
    void set_color_index( ColorIndex clr );
    void set_adjust_auto( bool b_auto ) { mb_auto_palatte = b_auto; }
    void set_level_span( float f_level, float f_level_span );

    void set_hor_ver( bool b_hor_ver );
    void set_hor_ver_image( bool b_hor_ver_image );
    void get_pelete_info( bool &b_auto, float &f_min_range, float &f_max_range, float &f_level, float &f_levelspan, ColorIndex &clr );

    QString get_clr_string( ColorIndex clr );
    void change_clr_index( QPoint pt );

    void draw_backgroud( QPainter &draw );
    void draw_clr_btn( QPainter &draw, QRect &rct );
    void draw_auto_btn( QPainter &draw, QRect &rct );
    void draw_range_top_rect( QPainter &draw, QRect &rct );
    void draw_range_bottom_rect( QPainter &draw, QRect &rct );
    void draw_palette( QPainter &draw, QRect &rct );

public:
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void resizeEvent( QResizeEvent *event );
    void paintEvent( QPaintEvent *event );
    bool eventFilter( QObject *obj, QEvent *event );

private:
    QImage *mp_image_bar;
    bool mb_recal_levelspan;
    bool mb_image_reverse;
    bool mb_auto_palatte;
    ColorIndex m_clr_index;

    float mf_min, mf_max;
    float mf_level, mf_level_span;
    float mf_scale_level, mf_scale_levelspan;
    QMutex m_mutex;

    QWidget *mp_cursor_wnd;

    bool mb_hor_ver;
    bool mb_move_update;

private:
    bool mb_mouse_press;
    QPoint m_pt_down;

    enum CursorPos
    {
        cpNULL=0,
        cpTop=1,
        cpCenter=2,
        cpBottom=3,
        cpLeft=4,
        cpRight=5
    };
    CursorPos		m_cp_mouse_pos; //鼠标的位置

signals:
    void sig_set_adjust_range( float f_level, float f_levelspan );
    void sig_set_adjust_clr( int n_clr_id );
    void sig_set_adjust_auto();

public slots:
    void slot_update_adjust( bool b_auto, float f_min, float f_max, float f_level, float f_level_span );
};

#endif // WNDPALETTE_H

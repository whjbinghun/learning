#include "wndpalette.h"
#include <QPainter>
#include <QColor>
#include <QMouseEvent>
#include <math.h>
#include <QDebug>
#include <QMenu>

#define BTN_HEIGHT 24
#define BTN_WIDTH 30

WndPalette::WndPalette(QWidget *parent) :
    QWidget(parent)
  , mp_image_bar( 0 )
  , mb_image_reverse( false )
  , mb_auto_palatte( true )
  , mf_min( 0.0 )
  , mf_max( 0.0 )
  , mf_level( 0.0 )
  , mf_level_span( 0.0 )
  , m_clr_index( clrNone )
  , mf_scale_level( 0.0 )
  , mf_scale_levelspan( 0.0 )
  , mb_mouse_press( false )
  , mp_cursor_wnd( 0 )
  , mb_recal_levelspan( false )
  , mb_hor_ver( false )
{
    mb_hor_ver = true;//表示水平
    set_hor_ver_image( mb_hor_ver );
    //installEventFilter( this );
}

WndPalette::~WndPalette()
{
    if ( mp_image_bar ) {
        delete mp_image_bar;
    }
}

void WndPalette::init_palette( ColorIndex clr, bool b_auto
                   , float f_min, float f_max, float f_level, float f_level_span )
{
    mb_auto_palatte = b_auto;
    mf_min = f_min;
    mf_max = f_max;
    mf_level = f_level;
    mf_level_span = f_level_span;

    set_color_index( clr );
}

void WndPalette::set_color_index( ColorIndex clr )
{
    {
        QMutexLocker lc( &m_mutex );
        m_clr_index = clr;
    }
    update();
}

QString WndPalette::get_clr_string( ColorIndex clr )
{
    QString str;

    switch ( clr ) {
    case clrIron: str = "铁红"; break;
    case clrRainbow: str = "彩虹"; break;
    case clrFeather: str = "医疗"; break;
    case clrGray: str = "灰度"; break;
    case  clrIronReverse: str = "反转铁红"; break;
    case clrRainbowReverse: str = "反转彩虹"; break;
    case clrFeatherReverse: str = "反转医疗"; break;
    case  clrGrayReverse: str = "反转灰度"; break;
    default:
        break;
    }

    return str;
}

void WndPalette::paintEvent( QPaintEvent *event )
{

    QPainter draw;
    draw.begin( this );

    int n_btn_height( BTN_HEIGHT );
    int n_btn_width( BTN_WIDTH );
    QRect rct = rect();
    QRect rct_top, rct_clr_btn, rct_auto_btn, rct_bottom, rct_adjust_bar;
    if( !mb_hor_ver ) {
        rct_top = QRect( 0, 0, rct.right(), n_btn_height );
        rct_clr_btn = QRect( 0, rct.bottom() - n_btn_height, rct.right(), n_btn_height );
        rct_auto_btn = QRect( 0, rct_clr_btn.top() - n_btn_height - 2, rct.right(), n_btn_height );
         rct_bottom = QRect( 0, rct_auto_btn.top() - n_btn_height - 2, rct.right(), n_btn_height );

        rct_adjust_bar = QRect( 0, rct_top.bottom() + 3, rct.right(), rct_bottom.top() - rct_top.bottom() - 4 );
    } else {
        rct_top = QRect( rct.right()-n_btn_width*3-4, 0,n_btn_width, rct.height()-1 );//右边
        rct_clr_btn = QRect( rct.right()-n_btn_width, 0,n_btn_width, rct.height()-1 );
        rct_auto_btn = QRect( rct.right()-n_btn_width*2-2, 0,n_btn_width, rct.height()-1 );
        rct_bottom = QRect( 0, 0, n_btn_width, rct.height()-1 );//左边

        rct_adjust_bar = QRect( n_btn_width+2, 0, rct.right()-n_btn_width*4-7, rct.height()-1 );
    }
    draw_backgroud( draw );
    draw_range_top_rect( draw, rct_top);
    draw_clr_btn( draw, rct_clr_btn );
    draw_auto_btn( draw, rct_auto_btn );
    draw_range_bottom_rect( draw, rct_bottom );
    draw_palette( draw, rct_adjust_bar );
    draw.end();
}

void WndPalette::draw_backgroud( QPainter &draw )
{
#define clr_backgroud QColor( 18, 117, 122 )
    draw.fillRect( rect(), clr_backgroud );
}

void WndPalette::draw_clr_btn( QPainter &draw, QRect &rct )
{
    QString str_clr;

    {
        QMutexLocker lc( &m_mutex );
        str_clr = get_clr_string( m_clr_index );
    }

    draw.fillRect( rct, QColor( 30, 64, 71 ) );
    draw.setPen( QColor( 0, 0, 0 ) );
    draw.drawRect( rct );

    draw.setPen( QColor( 255, 255, 255 ) );
    draw.drawText( rct, Qt::AlignCenter, str_clr );
}

void WndPalette::draw_auto_btn( QPainter &draw, QRect &rct )
{
    QString str_auto;
    {
        QMutexLocker lc( &m_mutex );
        str_auto = mb_auto_palatte?"自动":"手动";
    }

    draw.fillRect( rct, QColor( 30, 64, 71 ) );
    draw.setPen( QColor( 0, 0, 0 ) );
    draw.drawRect( rct );
    draw.setPen( QColor( 255, 255, 255 ) );
    draw.drawText( rct, Qt::AlignCenter, str_auto );
}

void WndPalette::draw_range_top_rect( QPainter &draw, QRect &rct )
{
    float f_max;
    {
        QMutexLocker lc( &m_mutex );
        f_max = mf_level_span;
    }
    int n_int = ( int )f_max;
    int n_decimal = ( f_max - ( ( int )f_max ) ) * 10;
    if( n_decimal < 0 ) {
        n_decimal = -n_decimal;
    }
    QString str = QString("%1.%2").arg( n_int ).arg( n_decimal );

    draw.setPen( QColor( 0, 0, 0 ) );
    draw.drawRect( rct );
    draw.drawText( rct, Qt::AlignCenter, str );
}

void WndPalette::draw_range_bottom_rect( QPainter &draw, QRect &rct )
{
    float f_min;
    {
        QMutexLocker lc( &m_mutex );
        f_min = mf_level;
    }
    int n_int = ( int )f_min;
    int n_decimal = ( f_min - ( ( int )f_min ) ) * 10;
    if( n_decimal < 0 ) {
        n_decimal = -n_decimal;
    }
    QString str = QString("%1.%2").arg( n_int ).arg( n_decimal );

    draw.setPen( QColor( 0, 0, 0 ) );
    draw.drawRect( rct );
    draw.drawText( rct, Qt::AlignCenter, str );
}

void WndPalette::draw_palette( QPainter &draw, QRect &rct )
{
    QMutexLocker lc( &m_mutex );
    if ( m_clr_index == clrNone || mf_max == mf_min || mf_level == mf_level_span ){
        return;
    }

    if ( mb_recal_levelspan || mb_auto_palatte || mf_scale_level == mf_scale_levelspan || mf_scale_level == 0.0 ) {
        if ( mb_recal_levelspan ) { mb_recal_levelspan = false; }
        float f_range = mf_max - mf_min;
        mf_scale_level = (mf_level - mf_min) / f_range;
        mf_scale_levelspan = ( mf_level_span - mf_min ) / f_range;
    }

    int n_bottom = rct.height() - rct.height() * mf_scale_level;
    int n_top = rct.height() - rct.height() * mf_scale_levelspan;

    int n_clr = ( int )m_clr_index-1;

    if( !mb_hor_ver ) {
        if( n_top<=0 ) {
            n_top=1;
        }
        if( n_bottom>=rct.height() ) {
            n_bottom = rct.height()-1;
        }
        draw.drawImage( QRect( rct.left(), rct.top()+n_top, rct.width(), n_bottom-n_top ),
                       *mp_image_bar, QRect( 0, n_clr*50, 16, 50 ) );
    } else {
        int n_left= rct.width() * mf_scale_level;
        int n_right = rct.width() * mf_scale_levelspan;
        if( n_right>=rct.width() ) {
            n_right = rct.width()-1;
        }
        if( n_left<=0 ) {
            n_left = 1;
        }
        draw.drawImage( QRect( rct.left()+n_left, rct.top(), n_right-n_left, rct.height() ),
                       *mp_image_bar, QRect( mp_image_bar->width()-m_clr_index*50, 0, 50, 16 ) );
    }
}

void WndPalette::resizeEvent( QResizeEvent *event )
{
}

void WndPalette::mousePressEvent( QMouseEvent *event )
{
    QPoint pt = event->pos();
    if ( cpNULL != m_cp_mouse_pos ) {
        mb_mouse_press = true;
        m_pt_down = pt;
    } else {
        int n_btn_height( BTN_HEIGHT );
        int n_btn_width( BTN_WIDTH );
        QRect rct = rect();
        if( !mb_hor_ver ) {
            if ( pt.y() < rct.bottom() && pt.y() > rct.bottom() - n_btn_height ) {
                change_clr_index( event->globalPos() );//改变颜色
            } else if ( pt.y() < rct.bottom() - n_btn_height && pt.y() >  rct.bottom() - n_btn_height*2 - 2 ) {
                emit sig_set_adjust_auto();//自动
            }
        } else {
            if ( pt.x() < rct.right() && pt.x() > rct.right()-n_btn_width ) {
                change_clr_index( event->globalPos() );
            } else if ( pt.x() <  rct.right()-n_btn_width && pt.x() >  rct.right()-n_btn_width*2-2 ) {
                emit sig_set_adjust_auto();
            }
        }
    }
}

void WndPalette::mouseMoveEvent( QMouseEvent *event )
{
    int n_btn_height( BTN_HEIGHT );
    int n_btn_width( BTN_WIDTH );
    QPoint pt = event->pos();

    QRect rct_adjust_bar;//调色板中图片所在的区域
    float f_range = mf_max - mf_min;
    float f_scale_levelspan, f_scale_level;
    float f_max, f_min;
    bool b_change_min(false), b_change_max(false);
    if( !mb_hor_ver ) {//垂直
        rct_adjust_bar = QRect( 0, n_btn_height + 2, width(), height() - 4 * n_btn_height - 6 );

        int n_top = rct_adjust_bar.top() + rct_adjust_bar.height() - mf_scale_levelspan * rct_adjust_bar.height() ;
        int n_bottom = rct_adjust_bar.top() + rct_adjust_bar.height() - mf_scale_level * rct_adjust_bar.height();

        if ( mb_mouse_press ) {
            switch( m_cp_mouse_pos )
            {
            case cpTop:
                {
                    if ( pt.y() > rct_adjust_bar.top() ) {
                        f_scale_levelspan = ( float )( rct_adjust_bar.height() - pt.y() + rct_adjust_bar.top() ) / rct_adjust_bar.height();
                        f_max = f_scale_levelspan*f_range + mf_min;

                        if ( f_scale_levelspan <= mf_scale_level || f_max < f_min ) return;
                        b_change_max = true;
                    }
                }
                break;
            case cpBottom:
                {
                    if ( pt.y() < rct_adjust_bar.bottom() ) {
                        f_scale_level = ( float )( rct_adjust_bar.height() - pt.y() + rct_adjust_bar.top() ) / rct_adjust_bar.height();
                        f_min = f_scale_level*f_range + mf_min;

                        if ( f_scale_level >= mf_scale_levelspan || f_min > mf_max ) return;
                        b_change_min = true;
                    }
                }
                break;
            case cpCenter:
                {
                    int n_sep = pt.y() - m_pt_down.y();
                    m_pt_down = pt;

                    qDebug() << "n_sep" <<(int)n_sep << mf_scale_level << mf_scale_levelspan;

                    int n_top = rct_adjust_bar.height() - mf_scale_levelspan*rct_adjust_bar.height() + n_sep+(n_sep>0?0.5:0.5) ;
                    int n_bottom = rct_adjust_bar.height() - mf_scale_level*rct_adjust_bar.height() + n_sep+(n_sep>0?0.5:0.5);

                    if ( n_top < 1 ) {
                        n_bottom -= n_top - 1;
                        n_top = 1;
                    } else if ( n_bottom > rct_adjust_bar.height()-1) {
                        n_top -= n_bottom - rct_adjust_bar.height() + 1;
                        n_bottom = rct_adjust_bar.height() - 1;
                    }

                    qDebug() << "n_sep" <<(int)n_sep << n_top << n_bottom;
                    if ( n_top < 0 ) { n_top = 0; }
                    if ( n_bottom > rct_adjust_bar.height() - 1 ) { n_bottom = rct_adjust_bar.height() - 1; }

                    f_scale_level = ( float )( rct_adjust_bar.height() - n_bottom ) / rct_adjust_bar.height();
                    f_min = f_scale_level*f_range + mf_min;

                    f_scale_levelspan = ( float )( rct_adjust_bar.height() - n_top) / rct_adjust_bar.height();
                    f_max = f_scale_levelspan*f_range + mf_min;

                    b_change_max = true;
                    b_change_min = true;
                }
                break;
            default:
                return;
            }

            {
                QMutexLocker lc( &m_mutex );
                if ( b_change_max && b_change_min ) {
                    mf_scale_level = f_scale_level;
                    mf_level = f_min;
                    mf_scale_levelspan = f_scale_levelspan;
                    mf_level_span = f_max;
                } else if ( b_change_max ) {
                    mf_scale_levelspan = f_scale_levelspan;
                    mf_level_span = f_max;
                } else if ( b_change_min ) {
                    mf_scale_level = f_scale_level;
                    mf_level = f_min;
                } else {
                    return;
                }

                if ( mf_level_span - mf_level < 1.0 ) {
                    if ( mf_max - mf_level_span > mf_level - mf_min ) {
                        mf_level_span = mf_level + 1.0;
                    } else {
                        mf_level = mf_level_span - 1.0;
                    }
                    mb_recal_levelspan = true;
                }
            }
            update();
        } else {
            if ( abs( event->y() - n_top ) < 5 ) {
                m_cp_mouse_pos = cpTop;
                //VodPlayInterface::SetCursorStyle( mp_cursor_wnd, VodPlayInterface::eTop );
            } else if ( abs( event->y() - n_bottom ) < 5 ) {
                m_cp_mouse_pos = cpBottom;
                //VodPlayInterface::SetCursorStyle( mp_cursor_wnd, VodPlayInterface::eTop );
            } else if ( event->y() > n_top + 5 && event->y() < n_bottom - 5 ) {
                m_cp_mouse_pos = cpCenter;
                //VodPlayInterface::SetCursorStyle( mp_cursor_wnd, VodPlayInterface::eMid );
            } else {
                m_cp_mouse_pos = cpNULL;
                //VodPlayInterface::SetCursorStyle( mp_cursor_wnd, VodPlayInterface::eNone );
            }
        }
     } else {//水平
        rct_adjust_bar = QRect( n_btn_width+2, 0, width()-4*n_btn_width-6, height() );

        int n_left = rct_adjust_bar.left() +mf_scale_level * rct_adjust_bar.width();
        int n_right = rct_adjust_bar.left()  + mf_scale_levelspan * rct_adjust_bar.width();

        if ( mb_mouse_press ) {
            switch( m_cp_mouse_pos )
            {
            case cpLeft:
                {
                    if( pt.x() > rct_adjust_bar.left() ) {
                        f_scale_level = ( float )( pt.x() - rct_adjust_bar.left() ) / rct_adjust_bar.width();
                        f_min = f_scale_level*f_range + mf_min;

                        if ( f_scale_level >= mf_scale_levelspan || f_min > mf_max ) return;
                        b_change_min = true;
                    }
                }
                break;
            case cpRight:
                {
                    if( pt.x() < rct_adjust_bar.right() ) {
                        f_scale_levelspan = ( float )(  pt.x() - rct_adjust_bar.left() ) / rct_adjust_bar.width();
                        f_max = f_scale_levelspan*f_range + mf_min;

                        if( f_scale_levelspan <= mf_scale_level || f_max < f_min ) return;
                        b_change_max = true;
                    }
                }
                break;
            case cpCenter:
                {
                    int n_sep = pt.x() - m_pt_down.x();
                    m_pt_down = pt;

                    qDebug() << "n_sep" <<(int)n_sep << mf_scale_level << mf_scale_levelspan;

                    int n_left = mf_scale_level*rct_adjust_bar.width() + n_sep+(n_sep>0?0.5:0.5) ;
                    int n_right = mf_scale_levelspan*rct_adjust_bar.width() + n_sep+(n_sep>0?0.5:0.5);

                    if ( n_left < 1 ) {
                        n_right -= n_left - 1;
                        n_left = 1;
                    } else if ( n_right > rct_adjust_bar.width()-1) {
                        n_left -= n_right - rct_adjust_bar.width() + 1;
                        n_right = rct_adjust_bar.width() - 1;
                    }

                    qDebug() << "n_sep" <<(int)n_sep << n_left << n_right;
                    if ( n_left < 0 ) { n_left = 0; }
                    if ( n_right > rct_adjust_bar.width() - 1 ) { n_right = rct_adjust_bar.width() - 1; }

                    f_scale_level = ( float )n_left / rct_adjust_bar.width();
                    f_min = f_scale_level*f_range + mf_min;

                    f_scale_levelspan = ( float )n_right / rct_adjust_bar.width();
                    f_max = f_scale_levelspan*f_range + mf_min;

                    b_change_max = true;
                    b_change_min = true;
                }
                break;
            default:
                return;
            }

            {
                QMutexLocker lc( &m_mutex );
                if ( b_change_max && b_change_min ) {
                    mf_scale_level = f_scale_level;
                    mf_level = f_min;
                    mf_scale_levelspan = f_scale_levelspan;
                    mf_level_span = f_max;
                } else if ( b_change_max ) {
                    mf_scale_levelspan = f_scale_levelspan;
                    mf_level_span = f_max;
                } else if ( b_change_min ) {
                    mf_scale_level = f_scale_level;
                    mf_level = f_min;
                } else {
                    return;
                }

                if ( mf_level_span - mf_level < 1.0 ) {
                    if ( mf_max - mf_level_span > mf_level - mf_min ) {
                        mf_level_span = mf_level + 1.0;
                    } else {
                        mf_level = mf_level_span - 1.0;
                    }
                    mb_recal_levelspan = true;
                }
            }
            update();
        } else {
            if ( abs( event->x() - n_left ) < 5 ) {
                m_cp_mouse_pos = cpLeft;
                //VodPlayInterface::SetCursorStyle( mp_cursor_wnd, VodPlayInterface::eTop );
            } else if ( abs( event->x() - n_right ) < 5 ) {
                m_cp_mouse_pos = cpRight;
                //VodPlayInterface::SetCursorStyle( mp_cursor_wnd, VodPlayInterface::eTop );
            } else if ( event->x() > n_left + 5 && event->x() < n_right - 5 ) {
                m_cp_mouse_pos = cpCenter;
                //VodPlayInterface::SetCursorStyle( mp_cursor_wnd, VodPlayInterface::eMid );
            } else {
                m_cp_mouse_pos = cpNULL;
                //VodPlayInterface::SetCursorStyle( mp_cursor_wnd, VodPlayInterface::eNone );
            }
        }
    }

}

void WndPalette::mouseReleaseEvent( QMouseEvent *event )
{
    if ( mb_mouse_press ) {
        m_cp_mouse_pos = cpNULL;
        //VodPlayInterface::SetCursorStyle( mp_cursor_wnd, VodPlayInterface::eNone );

        mb_mouse_press = false;

        emit sig_set_adjust_range( mf_level, mf_level_span );
    }
}

bool WndPalette::eventFilter( QObject *obj, QEvent *event )
{
    if ( obj == this ) {
        if ( event->type() == QEvent::Leave ) {
            m_cp_mouse_pos = cpNULL;
            //VodPlayInterface::SetCursorStyle( mp_cursor_wnd, VodPlayInterface::eNone );
        }
    }
}

void WndPalette::slot_update_adjust(bool b_auto, float f_min, float f_max, float f_level, float f_level_span)
{
    if ( mb_mouse_press ) return;

    {
        QMutexLocker lc( &m_mutex );
        mb_auto_palatte = b_auto;
        float f_range = f_max - f_min;
        mf_min = f_min - f_range/2;
        mf_max = f_max + f_range/2;
        mf_level = f_level;
        mf_level_span = f_level_span;

        mb_recal_levelspan = true;
    }

    update();
}

void WndPalette::change_clr_index( QPoint pt )
{
    QMenu menu(this);

    int n_clr = (int)m_clr_index;

    for( int n=1; n<9; ++n ) {
        QString str = get_clr_string( (ColorIndex)n );
        if ( str.isEmpty() ) continue;

        QAction *p_act = menu.addAction( str );
        p_act->setData( n );
        p_act->setCheckable( n_clr == n );
        p_act->setChecked( n_clr == n );
    }

    QAction *p_act = menu.exec( pt );
    if ( NULL == p_act ) return;

    int n_change_clr = p_act->data().toInt();
    emit sig_set_adjust_clr( n_change_clr );
}

void WndPalette::set_hor_ver( bool b_hor_ver )
{
    mb_hor_ver = b_hor_ver;
}

void WndPalette::set_hor_ver_image( bool b_hor_ver_image )
{
    if( !b_hor_ver_image ) {
        mp_image_bar  = new QImage(":image/image/palette.bmp", "BMP");
    } else {//水平
        mp_image_bar  = new QImage(":image/image/hor_palette.bmp", "BMP");
        QMatrix matrix;
        matrix.rotate(180);
        *mp_image_bar = mp_image_bar->transformed( matrix );
    }
}

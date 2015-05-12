#include "wndchart.h"
#include <QRect.h>
#include <math.h>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <QPixmap>
#include <QClipboard>
#include <QApplication>




WndChart::WndChart(QWidget *parent) :
    QWidget(parent)
  , mb_axisy_scroll( false )
  , mstr_title("")
  , mf_axisx_min( 0.0 )
  , mf_axisx_max( 80.0 )
  , mf_axisy_min( 0 )
  , mf_axisy_max( 20 )
  , mf_axisx_mark_span_temp( 10.0 )
  , mn_axisx_mark_span_pixel( 21 )
  , mn_axisx_mark_text_width( 20 )
  , mf_axisy_mark_span_pos( 21 )
  , mf_axisy_mark_span_pixel( 20 )
  , mn_axisy_scroll_left( 0 )
  , mn_axisy_scroll_len( 17 )
  , mn_mouse_down( mouse_down_null )
  , mn_mark_line_y(0)
  , mb_axisx_auto_range( true )
  , mp_show_all_trend( NULL )
  , mp_copy_trend( NULL )
  , mp_copy_temp( NULL )
  , mp_select_color( NULL )
{
    mf_axisy_min_scroll = mf_axisy_min;
    mf_axisy_max_scroll = mf_axisy_max;

    //mf_axisy_min_scroll = 83;
    //mf_axisy_max_scroll = 100;

    m_fnt_axis.setPixelSize( 12 );


    mp_show_all_trend = new QToolButton( this );
    mp_show_all_trend->setToolTipDuration( 1000 );
    mp_show_all_trend->setToolTip( "显示整体温度" );
    mp_show_all_trend->resize( 25, 25 );
    mp_show_all_trend->setIcon( QIcon( ":image/image/show.png") );
    mp_copy_trend = new QToolButton( this );
    mp_copy_trend->setIcon( QIcon( ":image/image/copy_ana.png") );
    mp_copy_trend->setToolTipDuration( 1000 );
    mp_copy_trend->setToolTip( "复制趋势曲线图" );
    mp_copy_trend->resize( 25, 25 );


    mp_copy_temp = new QToolButton( this );
    mp_copy_temp->setIcon( QIcon( ":image/image/copy_temp.png") );
    mp_copy_temp->setToolTipDuration( 1000 );
    mp_copy_temp->setToolTip( "复制趋势温度数据" );
    mp_copy_temp->resize( 25, 25 );

    mp_select_color = new QToolButton( this );
    mp_select_color->setIcon( QIcon( ":image/image/select_color.png") );
    mp_select_color->setToolTipDuration( 1000 );
    mp_select_color->setToolTip( "参数设置" );
    mp_select_color->resize( 25, 25 );
}

WndChart::~WndChart()
{
    if ( mp_show_all_trend ){
        delete mp_show_all_trend;
    }
    if ( mp_copy_trend ){
        delete mp_copy_trend;
    }
    if ( mp_copy_temp ){
        delete mp_copy_temp;
    }
}

void WndChart::mousePressEvent( QMouseEvent *event )
{
    if ( !mb_axisy_scroll ) return;

    if ( pt_in_chart( event->pos() ) ) {
        qDebug() << "pt_in_chart";
        mn_mouse_down = mouse_down_chart;
        m_pt_mouse_down = event->pos();
    } else if ( pt_in_scroll( event->pos() ) ) {
        qDebug() << "pt_in_scroll";
        mn_mouse_down = mouse_down_scroll;
        m_pt_mouse_down = event->pos();
    }
}

void WndChart::mouseMoveEvent( QMouseEvent *event )
{
    QPoint pt_move = event->pos();
    switch ( mn_mouse_down ) {
    case mouse_down_chart:
        move_chart( pt_move );
        m_pt_mouse_down = pt_move;
        break;
    case mouse_down_scroll:
        move_scroll( pt_move );
        m_pt_mouse_down = pt_move;
        break;
    default:
        move_mark_line( pt_move );
        break;
    }
}

void WndChart::mouseReleaseEvent( QMouseEvent *event )
{
    mn_mouse_down = mouse_down_null;
}

void WndChart::mouseDoubleClickEvent( QMouseEvent *event )
{
    QWidget::mouseDoubleClickEvent( event );
}

void WndChart::wheelEvent( QWheelEvent *event )
{
    int n_pos = event->delta() / 8 / 15;

    bool b_axisy_scroll = mb_axisy_scroll;
    if ( n_pos > 0 ) {
        if ( !mb_axisy_scroll ) return;
    } else {
        mb_axisy_scroll = true;
    }

    //float f_scale_pt = (float)( event->pos().x() - WIDTH_AXIS_X - mn_axisx_mark_text_width ) / ( width() - WIDTH_AXIS_X - SPACE_LEN - mn_axisx_mark_text_width);

    float f_offset_min( 0.0 ), f_offset_max( 0.0 );

    if ( b_axisy_scroll ) {
        f_offset_min = (float) ( mn_mark_line_y - mf_axisy_min_scroll ) *  n_pos / 10.0 ;
        f_offset_max = (float) ( mf_axisy_max_scroll - mn_mark_line_y ) *  n_pos / 10.0 ;
    } else {
        f_offset_min = (float) ( mn_mark_line_y - mf_axisy_min ) *  n_pos / 10.0 ;
        f_offset_max = (float) ( mf_axisy_max - mn_mark_line_y ) *  n_pos / 10.0 ;
    }
/*
    if ( f_scale_pt <= 0.0 ) {
        f_scale_pt = 0.01;
    } else if ( f_scale_pt >= 1.0 ) {
        f_scale_pt = 0.95;
    }

    //qDebug() << "wheelEvent" << f_offset_min << f_offset_max << mn_mark_line_y << mf_axisy_min_scroll << mf_axisy_max_scroll;
*/
    float f_offset;
    if ( b_axisy_scroll ) {
        f_offset =  ( mf_axisy_max_scroll - mf_axisy_min_scroll ) * ( n_pos / 10.0 );
    } else {
        f_offset =  ( mf_axisy_max - mf_axisy_min ) * ( n_pos / 10.0 );
        mf_axisy_max_scroll = mf_axisy_max;
        mf_axisy_min_scroll  = mf_axisy_min;
    }


    if ( n_pos <= 0 && f_offset < 1 && f_offset > -1 ) return;

    //move_scroll( 0-f_offset*f_scale_pt, f_offset*(1.0 -f_scale_pt) );
    move_scroll( 0-f_offset_min, f_offset_max );

    if ( mf_axisy_max_scroll == mf_axisy_max && mf_axisy_min_scroll == mf_axisy_min ) {
        mb_axisy_scroll = false;
    }

    cal_axis_y_span();
    update_axis_y();

}

void WndChart::resizeEvent( QResizeEvent *event )
{
    mp_show_all_trend->move( width() - 4 * mp_show_all_trend->width() - 10, 4 );
    mp_copy_trend->move( width() - 3 * mp_copy_trend->width() - 10, 4 );
    mp_copy_temp->move( width() - 2 * mp_copy_temp->width() - 10, 4 );
    mp_select_color->move( width() - mp_select_color->width() - 10, 4 );

    cal_axis_x_span();
    cal_axis_x_mark_info();

    cal_axis_y_span();

    cal_axis_y_mark_info();
    cal_axis_y_scroll();
}

void WndChart::cal_axis_y_scroll()
{
    if ( !mb_axisy_scroll ) return;

    float f_range_scroll = mf_axisy_max_scroll - mf_axisy_min_scroll;
    float f_range = mf_axisy_max - mf_axisy_min;

    int n_pix_range = width() - WIDTH_AXIS_X - SPACE_LEN - mn_axisx_mark_text_width - 4;
    int n_pix_scroll = n_pix_range * f_range_scroll / f_range;

    if ( n_pix_scroll < 17 ) { n_pix_scroll = 17; }

    mn_axisy_scroll_left = ( n_pix_range - n_pix_scroll ) * mf_axisy_min_scroll / ( f_range - f_range_scroll );
    mn_axisy_scroll_len = n_pix_scroll;
}

void WndChart::cal_axis_x_span()
{
    int ar_not[] = {1, 2, 5};

    /*static*/ int n_not = 0;
    /*static*/ int n_weight = 1;

    int n_width = width();
    int n_height = height();
    if ( n_width < 10 || n_height <10 ) { return; }
    QFontMetrics font_mts( m_fnt_axis );

    int a = height();
    int n_pixel = n_height - HEIGHT_TITLE - HEIGHT_TIME - HEIGHT_AXIS_Y - font_mts.height();
    n_pixel -= mb_axisy_scroll?WIDTH_AXIS_scroll:0;
    if ( n_pixel <= 0 ) { return; }
    float f_span = mf_axisx_max - mf_axisx_min;

    while (true)
    {
        mf_axisx_mark_span_temp = (float)( pow(10.0, n_weight) * ar_not[n_not] );
        mn_axisx_mark_span_pixel = (int)(n_pixel*mf_axisx_mark_span_temp/f_span);

        if ( mn_axisx_mark_span_pixel<19 ){
            n_not++;
            if (n_not>2){
                n_not = 0;
                n_weight++;
            }
        }else if ( mn_axisx_mark_span_pixel>51 ){
            n_not--;
            if (n_not<0){
                n_not = 2;
                n_weight--;
            }
        }else{
            //TRACE("span pixel %d temp %d\n", mn_axisx_mark_span_pixel, mf_axisx_mark_span_temp);
            break;
        }
    }
}

void WndChart::cal_axis_y_span()
{
    int ar_not[] = {1, 2, 5};

    /*static*/ int n_not = 0;
    /*static*/ int n_weight = 1;

    int n_width = width();
    int n_height = height();
    if (  n_width < 10 || n_height <10 ) { return; }
    QFontMetrics font_mts( m_fnt_axis );

    int a = n_height;
    int n_pixel = n_width - WIDTH_AXIS_X - SPACE_LEN - mn_axisx_mark_text_width;
    if ( n_pixel <= 0 ) { return; }
    float f_span = !mb_axisy_scroll? mf_axisy_max - mf_axisy_min : mf_axisy_max_scroll - mf_axisy_min_scroll;

    while (true)
    {
        mf_axisy_mark_span_pos = ( pow(10.0, n_weight) * ar_not[n_not] );
        mf_axisy_mark_span_pixel = (int)(n_pixel*mf_axisy_mark_span_pos/f_span);

        if ( mf_axisy_mark_span_pixel<39 ){
            n_not++;
            if (n_not>2){
                n_not = 0;
                n_weight++;
            }
        }else if ( mf_axisy_mark_span_pixel>101 ){
            n_not--;
            if (n_not<0){
                n_not = 2;
                n_weight--;
            }
        }else{
            //TRACE("span pixel %d temp %d\n", mn_axisx_mark_span_pixel, mf_axisx_mark_span_temp);
            break;
        }
    }
}

void WndChart::paintEvent( QPaintEvent *event )
{
    QPainter draw;

    draw.begin( this );

    draw_title( draw );
    draw_chart( draw );
    draw_axis( draw );
    draw_time( draw );

    draw.end();
}

void WndChart::draw_axis( QPainter &draw )
{
    QFontMetrics font_mts( m_fnt_axis );
    int n_height_txt = font_mts.height();

    int n_width_scroll = ( mb_axisy_scroll?WIDTH_AXIS_scroll:0 );
    int n_pix_v = height() - HEIGHT_TITLE - HEIGHT_TIME - HEIGHT_AXIS_Y - n_height_txt - n_width_scroll;
    int n_pix_h = width() - WIDTH_AXIS_X - SPACE_LEN - mn_axisx_mark_text_width;

    int n_width_x = mn_axisx_mark_text_width+WIDTH_AXIS_X;

    QRect rct_axis_x( 0, HEIGHT_TITLE, n_width_x, n_pix_v );
    QRect rct_axis_y_scroll( n_width_x - 1, rct_axis_x.bottom(), n_pix_h, n_width_scroll );
    QRect rct_axis_y( n_width_x - 1, rct_axis_y_scroll.bottom() + 1, n_pix_h, HEIGHT_AXIS_Y  );

    draw.setFont( m_fnt_axis );
    draw_axis_x( draw, rct_axis_x );
    draw_axis_y_scroll( draw, rct_axis_y_scroll );
    draw_axis_y( draw, rct_axis_y );
}

void WndChart::draw_chart( QPainter &draw )
{
    QFontMetrics font_mts( m_fnt_axis );
    int n_height_txt = font_mts.height();

    int n_width_scroll = ( mb_axisy_scroll?WIDTH_AXIS_scroll:0 );
    int n_pix_v = height() - HEIGHT_TITLE - HEIGHT_TIME - HEIGHT_AXIS_Y - n_height_txt - n_width_scroll;
    int n_pix_h = width() - WIDTH_AXIS_X - SPACE_LEN - mn_axisx_mark_text_width;

    int n_width_x = mn_axisx_mark_text_width+WIDTH_AXIS_X;
    QRect rct_chart( n_width_x-1, HEIGHT_TITLE-1,  n_pix_h+1, n_pix_v+1 );

    QPixmap bmp_pix( rct_chart.size() );
    QPainter draw_buf( &bmp_pix );
    draw_buf.initFrom( this );

    draw_axis_backgroud( draw_buf, QRect( QPoint(0, 0), rct_chart.size() ) );
    draw_chart_line( draw_buf, QRect( QPoint(0, 0), rct_chart.size() ) );

    draw.drawPixmap( rct_chart, bmp_pix );
}

void WndChart::draw_title( QPainter &draw )
{
    QRect rct_title( SPACE_LEN, 0, width(), HEIGHT_TITLE );

    draw.setPen( QColor( 132, 148, 99 ) );
    draw.drawRect( QRect(0, 0, width()-1, height()-1 ) );
    draw.setPen( QColor( 10, 10, 10 ) );
    draw.drawText( rct_title, Qt::AlignVCenter, mstr_title );
}

void WndChart::draw_time( QPainter &draw )
{
    QRect rct( 0, height() - HEIGHT_AXIS_Y - HEIGHT_TIME, width(), HEIGHT_AXIS_Y + HEIGHT_TIME );

    draw_chart_time( draw, rct );
}

void WndChart::draw_chart_time( QPainter &draw, const QRect &rct )
{

}

void WndChart::draw_axis_x( QPainter &draw, const QRect &rct )
{
    QFontMetrics font_mts( m_fnt_axis );
    int n_height_txt = font_mts.height();
    int n_width_txt = mn_axisx_mark_text_width;
    float f_minor_pixel = ((float)mn_axisx_mark_span_pixel)/3.0;	//.次要刻度的间距

    draw.setPen( COLOR_AXIS );
    draw.drawLine( rct.topRight(), rct.bottomRight() );

    QList< MarkInfo >::iterator it = m_list_axisx_mark.begin();
    for( ; it != m_list_axisx_mark.end(); ++it ) {
        draw.setPen( COLOR_AXIS );
        QPoint pt1( rct.right(), rct.bottom() - it->n_pix );
        QPoint pt2( rct.right() - LEN_MARK, rct.bottom() - it->n_pix );
        draw.drawLine( pt1, pt2 );

        for( int n=1; n<3; ++n ) {
            int n_minor = (int)(pt1.y() - n*f_minor_pixel);
            if ( n_minor<HEIGHT_TITLE ) break;
            QPoint pt_s( pt1.x(), n_minor );
            QPoint pt_e( rct.right() - LEN_MARK_MAJOR, n_minor );
            draw.drawLine( pt_s, pt_e );
        }

        draw.setPen( QColor( 10, 10, 10 ) );
        QRect rct_text( pt1.x() - SPACE_AXIS_TO_TXT - n_width_txt, pt1.y() - ( n_height_txt>>1 )
                        , n_width_txt, n_height_txt );
        draw.drawText( rct_text, it->str_temp, Qt::AlignVCenter|Qt::AlignRight );
    }
}

void WndChart::draw_axis_y( QPainter &draw, const QRect &rct )
{
    QRect rct_axisy = rct;

    draw.setPen( COLOR_AXIS );
    draw.drawLine( rct_axisy.topLeft(), rct.topRight() );

    float f_minor_pixel = ((float)mf_axisy_mark_span_pixel)/5.0;	//.次要刻度的间距

    QList< MarkInfo >::Iterator it = m_list_axisy_mark.begin();
    for ( ; it != m_list_axisy_mark.end(); ++it ) {
        draw.setPen( COLOR_AXIS );
        QPoint pt1( rct_axisy.left() + it->n_pix, rct.top()  );
        QPoint pt2( rct_axisy.left() + it->n_pix, rct.top() + LEN_MARK );
        draw.drawLine( pt1, pt2 );

        for( int n=1; n<5; ++n ) {
            int n_minor = (int)(pt1.x() + n*f_minor_pixel);
            if ( n_minor > rct.right() ) break;
            QPoint pt_s( n_minor, pt1.y() );
            QPoint pt_e( n_minor, pt1.y() + LEN_MARK_MAJOR );
            draw.drawLine( pt_s, pt_e );
        }

        if ( it == m_list_axisy_mark.begin() ) {
            for( int n=1; n<5; ++n ) {
                int n_minor = (int)(pt1.x() - n*f_minor_pixel);
                if ( n_minor < rct.left() ) break;
                QPoint pt_s( n_minor, pt1.y() );
                QPoint pt_e( n_minor, pt1.y() + LEN_MARK_MAJOR );
                draw.drawLine( pt_s, pt_e );
            }
        }
    }
}

void WndChart::draw_axis_y_scroll( QPainter &draw, const QRect &rct )
{
    if ( !mb_axisy_scroll ) return;

    draw.setPen( COLOR_AXIS );
    draw.drawRect( rct );

    QRect rct_scroll( rct.left() + 2 + mn_axisy_scroll_left, rct.top() + 2, mn_axisy_scroll_len, rct.height() - 3 );
    draw.fillRect( rct_scroll, COLOR_AXIS );

    draw.setPen( QColor(  255, 255, 255 ) );
    QPoint pt_center( rct_scroll.left() + rct_scroll.width()/2, rct_scroll.top() + rct_scroll.height()/2 );

    draw.drawLine( QPoint( pt_center.x(), pt_center.y() - 1 ), QPoint( pt_center.x(), pt_center.y() + 1 )  );
    draw.drawLine( QPoint( pt_center.x() - 2, pt_center.y() - 1 ), QPoint( pt_center.x() - 2, pt_center.y() + 1 )  );
    draw.drawLine( QPoint( pt_center.x() + 2, pt_center.y() - 1 ), QPoint( pt_center.x() + 2, pt_center.y() + 1 )  );
}

void WndChart::draw_axis_backgroud( QPainter &draw, const QRect &rct  )
{
    draw.fillRect( rct, QColor( 238, 238, 227 ) );

    draw.setPen( QColor(220, 221, 197) );
    QList< MarkInfo >::iterator it = m_list_axisx_mark.begin();
    for( ; it != m_list_axisx_mark.end(); ++it ) {
        if ( it->n_pix == 0 || rct.bottom() - it->n_pix <= 0) continue;

        QPoint pt1( rct.left(), rct.bottom() - it->n_pix );
        QPoint pt2( rct.right(), rct.bottom() - it->n_pix );
        draw.drawLine( pt1, pt2 );
    }

    it = m_list_axisy_mark.begin();
    for ( ; it != m_list_axisy_mark.end(); ++it ) {
        if ( it->n_pix == 0 || rct.left() + it->n_pix >= rct.right() ) continue;

        QPoint pt1( rct.left() + it->n_pix, rct.top() );
        QPoint pt2( rct.left() + it->n_pix, rct.bottom() );
        draw.drawLine( pt1, pt2 );
    }

    draw.setPen( QColor(203, 201, 164) );
    draw.drawLine( rct.topLeft(), rct.topRight() );
    draw.drawLine( rct.topRight(), rct.bottomRight() );
}

void WndChart::draw_chart_line( QPainter &draw, const QRect &rct  )
{

}

void WndChart::get_axis_x_mark_temp_and_pix( float &f_min_temp, int &n_min_pix )
{
    //.计算最低温度刻度位置
    n_min_pix = (int) (( mf_axisx_min/mf_axisx_mark_span_temp - ((int)(mf_axisx_min/mf_axisx_mark_span_temp))) * mn_axisx_mark_span_pixel);

    //.计算刻度的最低温度值
    f_min_temp = mf_axisx_min/mf_axisx_mark_span_temp;
    if (mf_axisx_min > 0.0){
        n_min_pix -= mn_axisx_mark_span_pixel;
        f_min_temp += 1;
    }
    f_min_temp = (float)(int)f_min_temp;
    f_min_temp	= f_min_temp*mf_axisx_mark_span_temp;

    n_min_pix = abs( n_min_pix );
}

void WndChart::cal_axis_x_mark_info()
{
    m_list_axisx_mark.clear();
    QFontMetrics font_mts( m_fnt_axis );

    int n_width_max_string(0);
    int n_min_pix;
    float f_min_temp_mark;
    get_axis_x_mark_temp_and_pix( f_min_temp_mark, n_min_pix );

    for( int n = n_min_pix; f_min_temp_mark<=mf_axisx_max; n += mn_axisx_mark_span_pixel ) {
        QString str;
        if ( ((int)f_min_temp_mark)*10 == (int)(f_min_temp_mark*10.0) ){
            str= QString("%1").arg( (int)f_min_temp_mark );
        }else{
            str= QString::number(f_min_temp_mark, 'f', 1);
        }

        int n_width_font = font_mts.width( str );
        n_width_max_string = n_width_font > n_width_max_string?n_width_font:n_width_max_string;

        f_min_temp_mark += mf_axisx_mark_span_temp;

        MarkInfo mark_info;
        mark_info.n_pix = n;
        mark_info.str_temp = str;
        mark_info.f_pos = f_min_temp_mark;
        m_list_axisx_mark.push_back( mark_info );

    }

    mn_axisx_mark_text_width = n_width_max_string;
}

void WndChart::get_axis_y_mark_pos_and_pix( float &f_min_pos, int &n_min_pix  )
{
    float n_min = !mb_axisy_scroll? mf_axisy_min : mf_axisy_min_scroll;
    float f_min = ( (float) n_min)/mf_axisy_mark_span_pos;
    n_min_pix = (int)(  ( f_min -  (int)( f_min )  ) * mf_axisy_mark_span_pixel );

    //.计算刻度的最低温度值
    f_min_pos = (int)(n_min/mf_axisy_mark_span_pos);
    if (n_min > 0.0){
        n_min_pix -= mf_axisy_mark_span_pixel;
        f_min_pos += 1;
    }
    f_min_pos	= f_min_pos*mf_axisy_mark_span_pos;

    n_min_pix = abs( n_min_pix );
}

void WndChart::get_axis_x_range(float &f_min, float &f_max)
{
    f_min = mf_axisx_min;
    f_max = mf_axisx_max;
}

void WndChart::cal_axis_y_mark_info()
{
    m_list_axisy_mark.clear();

    float f_max_pos = mb_axisy_scroll? mf_axisy_max_scroll : mf_axisy_max;
    int n_min_pix;
    float f_min_pos_mark;
    get_axis_y_mark_pos_and_pix( f_min_pos_mark, n_min_pix );

    for( float n = n_min_pix; f_min_pos_mark<=f_max_pos; n += mf_axisy_mark_span_pixel ) {
        QString str = QString("%1").arg( f_min_pos_mark );

        f_min_pos_mark += mf_axisy_mark_span_pos;

        MarkInfo mark_info;
        mark_info.n_pix = n;
        mark_info.str_temp = str;
        mark_info.f_pos = f_min_pos_mark;
        m_list_axisy_mark.push_back( mark_info );
    }
}

bool WndChart::pt_in_chart( const QPoint &pt )
{
    QFontMetrics font_mts( m_fnt_axis );
    int n_height_txt = font_mts.height();

    int n_width_scroll = ( mb_axisy_scroll?WIDTH_AXIS_scroll:0 );
    int n_pix_v = height() - HEIGHT_TITLE - HEIGHT_TIME - HEIGHT_AXIS_Y - n_height_txt - n_width_scroll;
    int n_pix_h = width() - WIDTH_AXIS_X - SPACE_LEN - mn_axisx_mark_text_width;

    int n_width_x = mn_axisx_mark_text_width+WIDTH_AXIS_X;
    QRect rct_chart( n_width_x-1, HEIGHT_TITLE-1,  n_pix_h+1, n_pix_v+1 );

    return rct_chart.contains( pt );
}

bool WndChart::pt_in_scroll( const QPoint &pt )
{
    QFontMetrics font_mts( m_fnt_axis );
    int n_height_txt = font_mts.height();

    int n_width_scroll = ( mb_axisy_scroll?WIDTH_AXIS_scroll:0 );
    int n_pix_v = height() - HEIGHT_TITLE - HEIGHT_TIME - HEIGHT_AXIS_Y - n_height_txt - n_width_scroll;
    int n_pix_h = width() - WIDTH_AXIS_X - SPACE_LEN - mn_axisx_mark_text_width;

    int n_width_x = mn_axisx_mark_text_width+WIDTH_AXIS_X;

    QRect rct_axis_y_scroll( n_width_x - 1, HEIGHT_TITLE + n_pix_v, n_pix_h, n_width_scroll );

    return rct_axis_y_scroll.contains( pt );
}

void WndChart::move_chart( const QPoint &pt )
{
    int n_offset_pix = m_pt_mouse_down.x() - pt.x();

    float f_offset_pos = ( ( float )n_offset_pix ) * mf_axisy_mark_span_pos / mf_axisy_mark_span_pixel;

    move_scroll( f_offset_pos, f_offset_pos );
    update_axis_y();
}

void WndChart::move_scroll( const QPoint &pt )
{
    int n_offset_pix = pt.x() - m_pt_mouse_down.x();

    float f_range_scroll = mf_axisy_max_scroll - mf_axisy_min_scroll;
    float f_range = mf_axisy_max - mf_axisy_min;

    int n_pix_range = width() - WIDTH_AXIS_X - SPACE_LEN - mn_axisx_mark_text_width - 4;

    float f_offset_pos = n_offset_pix * ( f_range - f_range_scroll ) / ( n_pix_range - mn_axisy_scroll_len );

    move_scroll( f_offset_pos, f_offset_pos );
    update_axis_y();
}

void WndChart::move_scroll( const float &f_offset_left, const float &f_offset_right )
{
    float f_min = mf_axisy_min_scroll + f_offset_left;
    float f_max = mf_axisy_max_scroll + f_offset_right;

    normalize_scroll( f_min, f_max );

    mf_axisy_min_scroll = f_min;
    mf_axisy_max_scroll = f_max;
}

void WndChart::move_mark_line(const QPoint &pt)
{
    int n_pix_pt = pt.x() - WIDTH_AXIS_X - mn_axisx_mark_text_width;

    int n_pix_range = width() - WIDTH_AXIS_X - SPACE_LEN - mn_axisx_mark_text_width;

    float f_range_min = mb_axisy_scroll ? mf_axisy_min_scroll : mf_axisy_min;
    float f_range_max = mb_axisy_scroll ? mf_axisy_max_scroll : mf_axisy_max;

   int n_mark_line_y = n_pix_pt * mf_axisy_mark_span_pos / mf_axisy_mark_span_pixel + f_range_min + 0.5;

   if ( n_mark_line_y != mn_mark_line_y && n_mark_line_y >= 0) {
       mn_mark_line_y = n_mark_line_y;
       show_tool_tip( pt, mn_mark_line_y );
       update();
   }
}

void WndChart::normalize_scroll(float &f_min, float &f_max)
{
    if ( f_min < mf_axisy_min ) {
        f_max -= f_min - mf_axisy_min;
        f_min = mf_axisy_min;
    } else if ( f_max > mf_axisy_max  ) {
        f_min -= f_max - mf_axisy_max;
        f_max = mf_axisy_max;
    }

    if ( f_min < mf_axisy_min ) { f_min = mf_axisy_min; }
    if ( f_max > mf_axisy_max  ) { f_max = mf_axisy_max; }
}

void WndChart::update_axis_y()
{
    cal_axis_y_mark_info();
    cal_axis_y_scroll();
    update();
}

void WndChart::update_axis_x()
{
    cal_axis_x_span();
    cal_axis_x_mark_info();
    update();
}

void WndChart::show_tool_tip(const QPoint &pt, int n_pos_y)
{

}

void WndChart::copy_chart()
{
    QClipboard *board = QApplication::clipboard();
    board->setPixmap( grab() );
}

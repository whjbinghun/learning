#include "wndcharttrend.h"
#include <qevent.h>
#include <qtooltip.h>
#include <qdebug.h>
#include "datachartmode.h"
#include <QThread>
#include <QApplication>
#include <QClipboard>
#include "wndanachartcolor.h"
#include "msanacolorsetting.h"
#include "wndloadpointpicture.h"
#include <QMessageBox>
#include "../common-funtion/msclientlog.h"

WndChartTrend::WndChartTrend(QWidget *parent) :
    WndChart(parent)
  , mb_first_cal_axisy( true )
  , mb_first_cal_axisx( true )
  , mb_realtime( true )
  , mn_group_num( 0 )
  , m_str_tooltip( "" )
  , mn_mark_line_pix( 0 )
  , mn_timer_update_axis( 0 )
  , mn_pos_max( 20 )
  , mf_max( -999999 )
  , mf_min( 999999 )
  , mn_double_click_mark_time( 0 )
  , mp_wnd_picture( NULL )
  , mn_preset_no( -1 )
  , m_str_preset_name( "" )
{
    mn_index =- 1;

    connect( mp_show_all_trend, SIGNAL( clicked() ), this, SLOT( slot_btn_show_all_trend() ) );
    connect( mp_copy_trend, SIGNAL( clicked() ), this, SLOT( slot_btn_copy_trend() ) );
    connect( mp_copy_temp, SIGNAL( clicked() ), this, SLOT( slot_btn_copy_temp() ) );
    connect( mp_select_color, SIGNAL( clicked() ), this, SLOT( slot_btn_select_color() ) );
    connect( this, SIGNAL( sig_load_ir_vi_picture( bool, QString, QString ) ), this, SLOT( slot_load_ir_vi_picture( bool, QString, QString ) ) );

    mp_timer_update = new QTimer(this);
    connect( mp_timer_update, SIGNAL( timeout() ), this, SLOT( on_timer_update_ana() ) );
    mp_timer_update->start( 100 );

    mn_timer_update_axis = startTimer( 100 );
    mstr_title = "Y：温度（单位 ℃） X：趋势分析的时间";

    mp_wnd_picture = new WndLoadPointPicture( this );

}

WndChartTrend::~WndChartTrend()
{
    if ( mn_timer_update_axis ){
        killTimer( mn_timer_update_axis );
        mn_timer_update_axis = 0;
    }

    if ( mp_wnd_picture ){
        delete mp_wnd_picture;
        mp_wnd_picture = NULL;
    }

}

bool WndChartTrend::event(QEvent *event)
{
//    if (event->type() == QEvent::ToolTip) {

//        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);

//        QToolTip::showText(helpEvent->globalPos(), m_str_tooltip, 0, rect(), 5000 );

//        return true;
//    }

    return WndChart::event(event);
}

void WndChartTrend::mouseMoveEvent( QMouseEvent *event )
{
    WndChart::mouseMoveEvent( event );
}

void WndChartTrend::timerEvent(QTimerEvent *event)
{
    if ( event->timerId() == mn_timer_update_axis ){

        const int n_span_temp = 2;
        int   n_pos_max( 20 );
        float f_temp_min( 999999 );
        float f_temp_max( -999999 );
//        bool b_first_add_chart( false );

        DeviceGroup  devGroup;
        if( ! CDataMode::Instance()->get_group_info( mn_group_num,devGroup ) ) return ;

        QString s_sn = devGroup.dev_ir.str_sn;
        if( s_sn== "" ) return;
        if( mb_realtime ) {
          s_sn+="_real";
        } else {
          s_sn+="_record";
        }

//        b_first_add_chart = CDataChartMode::Instance()->get_first_add( s_sn, mn_index );
//        if ( b_first_add_chart ){
//            mb_first_cal_axisy = true;
//            n_pos_max = 100;
//        }

        CDataChartMode::Instance()->get_max_min_value( s_sn, mn_index, n_pos_max, f_temp_min, f_temp_max );

        bool b_update_axisx(false);

        if ( mb_axisx_auto_range ) {
            float f_min = mf_min;
            float f_max = mf_max;
            if ( mb_first_cal_axisx ) {
                f_min = 999999;
                f_max = -999999;
            }

            if ( f_temp_min < f_min ) {
                f_min = f_temp_min - n_span_temp;
                b_update_axisx = true;
            }

            // 解决问题：当纵轴最小值的大于温度最大值时， 自动会失效; 新增条件f_min < f_temp_max
            if ( (f_temp_max > f_max - n_span_temp) || (f_min < f_temp_max) ) {
                f_max = f_temp_max + n_span_temp;
                b_update_axisx = true;
            }

            if ( b_update_axisx && f_min < f_max ) {
                mb_first_cal_axisx = false;
                mf_axisx_min = f_min;
                mf_axisx_max = f_max;
            }
        }

        // b_update_axisx
        if ( b_update_axisx ) {
            cal_axis_x_span();
            cal_axis_x_mark_info();
        }

        if ( mb_first_cal_axisy ) {
            qDebug() << "mb_first_cal_axisy" ;
            mb_first_cal_axisy = false;
            mf_axisy_max = 20;
            cal_axis_y_span();
            cal_axis_y_mark_info();
            cal_axis_y_scroll();
        } else {
//            if ( n_pos_max > mf_axisy_max ) {
                mf_axisy_max = n_pos_max;
                cal_axis_y_span();
                cal_axis_y_mark_info();
                cal_axis_y_scroll();
//            }

        }

        update();
    }
}

void WndChartTrend::mouseDoubleClickEvent( QMouseEvent *event )
{
    WndChart::mouseDoubleClickEvent( event );

    QString s_sn = get_sn();
    if ( s_sn.isEmpty() ) return;

    int n_sec ( 0 ), n_msec( 0 );
    if ( CDataChartMode::Instance()->get_pos_time( s_sn, mn_index, mn_mark_line_y, n_sec, n_msec ) ){

        mn_double_click_mark_time = n_sec + n_msec / 1000 ;
        get_pic_on_time( mn_group_num, n_sec, n_msec );
    } else {
        emit sig_load_ir_vi_picture( false, "", "" );
    }
}

int WndChartTrend::onResponse( CBaseMsg &reqMsg, CBaseMsg &rspMsg )
{
    if( rspMsg.m_nCmd == PicGetOnTime ){
        if ( rspMsg.m_nResult == ResultOk ) {
            QString str_ir_path = rspMsg.map_msgdata[ MSGDATA_IR_PATH ].toString();
            QString str_vi_path = rspMsg.map_msgdata[ MSGDATA_VI_PATH ].toString();
            emit sig_load_ir_vi_picture( true, str_ir_path, str_vi_path );
        } else {
            emit sig_load_ir_vi_picture( false, "", "" );
        }
    }
}

void WndChartTrend::on_timer_update_ana()
{
    //if ( mp_delegate==NULL || ( mp_delegate!= NULL &&  mp_delegate->is_empty()) ) {
    //    update();
    //    return;
    //}

     const int n_span_temp = 2;
     int n_pos_max(20);//n_pos_max( mf_axisy_max );
     float f_temp_min( 999999 ), f_temp_max( -999999 );
     bool b_min_change( false ), b_max_change( false );

     DeviceGroup  devGroup;
     if( ! CDataMode::Instance()->get_group_info(mn_group_num,devGroup) ) return ;

     QString s_sn = devGroup.dev_ir.str_sn;
     if( s_sn== "" ) return;
     if( mb_realtime ) {
       s_sn+="_real";
     } else {
       s_sn+="_record";
     }

    CDataChartMode::Instance()->get_max_min_valule_info( s_sn, mn_index, n_pos_max, f_temp_min, f_temp_max, b_min_change, b_max_change );

}

void WndChartTrend::on_chart_empty()
{
    mb_first_cal_axisy = true;
    mb_first_cal_axisx = true;
}

void WndChartTrend::draw_chart_line( QPainter &draw, const QRect &rct  )
{
    QList< ChartPoints * > list_charts;

    QString s_sn = get_sn();
    if ( s_sn.isEmpty() ) return;

    int list_index = 0;
    CDataChartMode::Instance()->get_data(s_sn,mn_index,list_charts);

    QList< ChartPoints * >::iterator it = list_charts.begin();
    for ( ; it != list_charts.end(); ++it ) {
        draw_pts( draw, rct, *it, list_index );
        list_index++;
    }

    draw_mark_line( draw, rct );
}

void WndChartTrend::draw_pts(QPainter &draw, const QRect &rct, ChartPoints *p_pts , int list_index )
{
    QColor clr_line;
    if ( get_color_by_index( list_index, clr_line ) ){

    }else{
        clr_line = p_pts->get_line_color();
        add_color_to_color_list( clr_line );
    }
    draw.setPen( clr_line );

    bool b_first( false );
    QPoint pt_start;

    const QList< ChartPointInfo >& list_pts = p_pts->get_pts();
    QList< ChartPointInfo >::const_iterator it = list_pts.begin();

    int n_pos_max( 0 );
    float f_temp_min( 0.0 ), f_temp_max( 0.0 );


    QString str_sn = get_sn();
    CDataChartMode::Instance()->get_max_min_value( str_sn, mn_index, n_pos_max, f_temp_min, f_temp_max );
    {
//        mf_axisy_max = n_pos_max;
//        cal_axis_y_span();
//        cal_axis_y_mark_info();
//        cal_axis_y_scroll();
    }

    int n_span_x = n_pos_max - list_pts.size();

    for ( int chart_index = 0; it != list_pts.end(); ++it, ++chart_index ) {
        float f_x( n_span_x + chart_index ), f_y( it->temp );

        int n_span = mf_axisy_mark_span_pos < 1.0 ? (1.0 / mf_axisy_mark_span_pos) : mf_axisy_mark_span_pos;
        if ( mb_axisy_scroll ) {
            if ( f_x < mf_axisy_min_scroll - n_span || f_x > mf_axisy_max_scroll + n_span ) {

                continue;
            }
        }

        f_x -= mb_axisy_scroll?mf_axisy_min_scroll:0;

        int n_x = f_x * mf_axisy_mark_span_pixel / mf_axisy_mark_span_pos;

        int n_y = ( f_y - mf_axisx_min ) * mn_axisx_mark_span_pixel / mf_axisx_mark_span_temp;
        n_y = rct.height() - n_y;

        if ( (int)n_span_x + chart_index == mn_mark_line_y ) {
            mn_mark_line_pix = n_x;
        }

        if ( b_first ) {
            draw.drawLine( pt_start, QPoint( n_x, n_y ) );
        } else {
            b_first = true;
        }
        pt_start = QPoint( n_x, n_y );
    }
}

void WndChartTrend::draw_mark_line( QPainter &draw , const QRect &rct  )
{
    draw.setPen( QColor( 255, 0, 0 ) );
    draw.drawLine( QPoint( mn_mark_line_pix, rct.top() ), QPoint( mn_mark_line_pix, rct.bottom() ) );
}

void WndChartTrend::show_tool_tip( const QPoint &pt, int n_pos_y )
{
    get_tooltip_string(n_pos_y, m_str_tooltip);
    if ( m_str_tooltip.trimmed().isEmpty() ){
        return ;
    }

    QToolTip::showText( mapToGlobal( pt ), m_str_tooltip, 0, rect(), 5000 );
    m_str_tooltip.clear();
}

void WndChartTrend::draw_chart_time( QPainter &draw, const QRect &rct )
{
    int n_x_start, n_x_end;
    if ( mb_axisy_scroll ) {
        n_x_start = mf_axisy_min_scroll;
        n_x_end = mf_axisy_max_scroll;
    } else {
        n_x_start = mf_axisy_min;
        n_x_end = mf_axisy_max;
    }

    QString str_start = get_time_string( n_x_start );
    QString str_end = get_time_string( n_x_end-1 );

    draw.setPen( QColor( 10, 10, 10 ) );
    draw.drawText( rct.left() + SPACE_TXT_TO_BRODER, rct.top() + SPACE_AXIS_TO_TXT, str_start );
    draw.drawText( rct.right() - SPACE_TXT_TO_BRODER - 120, rct.top() + SPACE_AXIS_TO_TXT, str_end );
}

void WndChartTrend::set_group_num(int n_num , int n_index)
{
    mn_group_num = n_num;
    mn_index = n_index;

    get_config_params();
    get_chart_color_list();
}

void WndChartTrend::set_type( bool b_realtime )
{
    mb_realtime = b_realtime;
}

void WndChartTrend::set_index( int n_index )
{
    mn_index = n_index;
}

int WndChartTrend::get_index()
{
   return mn_index;
}

void WndChartTrend::get_tooltip_string( int n_pos , QString &str_tool_tip )
{
    QString str_sn = get_sn();
    if ( str_sn.isEmpty() ) return;
    str_tool_tip = CDataChartMode::Instance()->get_tooltip_string( str_sn, mn_index, n_pos );

#ifdef QT_DEBUG
   // qDebug() << "tooltip_string: " << tool_tip;
#endif
}

QString WndChartTrend::get_time_string( int n_pos )
{
    QString str_sn = get_sn();
    if ( str_sn.isEmpty() ) return QString("");

    return CDataChartMode::Instance()->get_pos_time( str_sn, mn_index, n_pos );
}

QString WndChartTrend::get_sn()
{
    DeviceGroup  devGroup;
    if( ! CDataMode::Instance()->get_group_info( mn_group_num, devGroup ) )
        return "";

    QString s_sn = devGroup.dev_ir.str_sn;
    if( s_sn == "" )
        return "" ;

    if( mb_realtime ) {
      s_sn+="_real";
    } else {
      s_sn+="_record";
    }

    return s_sn;
}

void WndChartTrend::set_preset_name( QString &str_preset_name )
{
    m_str_preset_name = str_preset_name;
}

void WndChartTrend::get_chart_color_list()
{
    m_map_chart_color.clear();
    QStringList list_color = MSAnaColorSetting::Instance()->get_ana_color_keys( mn_group_num, mn_index );
    foreach ( QString str, list_color ) {
        QColor color = MSAnaColorSetting::Instance()->get_ana_chart_color( mn_group_num, mn_index, str );
        m_map_chart_color[ str ] = color;
    }
}

void WndChartTrend::set_chart_color_list()
{
    MSAnaColorSetting::Instance()->clear_ana_colors( mn_group_num, mn_index );
    QMap< QString, QColor >::iterator it = m_map_chart_color.begin();
    for( ; it != m_map_chart_color.end(); it++ ){
        MSAnaColorSetting::Instance()->set_ana_chart_color( mn_group_num, mn_index, it.key(), it.value() );
    }
    get_chart_color_list();
}

bool WndChartTrend::get_color_by_index( int list_index, QColor &color )
{
    bool b_ret = false;
    int map_index = 0;
    QMap< QString, QColor >::iterator it = m_map_chart_color.begin();
    for( ; it != m_map_chart_color.end(); it++ ){
        if ( map_index == list_index ){
            b_ret = true;
            color = it.value();
            break;
        }
        map_index++;
    }
    return b_ret;
}

void WndChartTrend::add_color_to_color_list( QColor &color )
{
    int map_size = m_map_chart_color.size();
    QString str_key = QString( "chart%1" ).arg( map_size + 100 + 1);
    m_map_chart_color[ str_key ] = color;
    MSAnaColorSetting::Instance()->set_ana_chart_color( mn_group_num, mn_index, str_key, color );
}

void WndChartTrend::get_config_params()
{
    QString str_path = QApplication::applicationDirPath() + "/config/ana_chart.ini" ;
    QSettings settings( str_path , QSettings::IniFormat, 0 );
    int n_temp_update_mode = settings.value( QString( "/%1-%2/update_mode" ).arg( mn_group_num ).arg( mn_index ), 0 ).toInt();
    float f_max = settings.value( QString( "/%1-%2/temp_max" ).arg( mn_group_num ).arg( mn_index ), 80.0 ).toFloat();
    float f_min = settings.value( QString( "/%1-%2/temp_min" ).arg( mn_group_num ).arg( mn_index ), 0.0 ).toFloat();
    int n_storage_time_interval_index = settings.value( QString( "/%1-%2/time_interval" ).arg( mn_group_num ).arg( mn_index ), 1 ).toInt();
    int n_display_time = settings.value( QString( "/%1-%2/clear_period" ).arg( mn_group_num ).arg( mn_index ), 3 ).toUInt();

    int nu_clear_period = 1 * 60 * 60;
    if ( n_display_time == 1 ){
        nu_clear_period = 2 * 60 * 60;
    } else if ( n_display_time == 2 ){
        nu_clear_period = 5 * 60 * 60;
    } else if ( n_display_time == 3 ){
        nu_clear_period = 12 * 60 * 60;
    } else if ( n_display_time == 4 ){
        nu_clear_period = 24 * 60 * 60;
    }

    if ( !n_temp_update_mode ){
        this->set_axis_x_auto_range( true );
    }else{
        this->set_axis_x_auto_range( false );
        this->set_axis_x_max( f_max );
        this->set_axis_x_min( f_min );
    }

    QString s_sn = get_sn();
    if ( s_sn.isEmpty() ) return;

    if ( n_storage_time_interval_index == 3 ){
        CDataChartMode::Instance()->set_collect_temp_interval_time( s_sn, mn_index, 5 );
    }else if ( n_storage_time_interval_index == 4 ){
        CDataChartMode::Instance()->set_collect_temp_interval_time( s_sn, mn_index, 10 );
    }else{
        CDataChartMode::Instance()->set_collect_temp_interval_time( s_sn, mn_index, n_storage_time_interval_index );
    }

    CDataChartMode::Instance()->set_chart_clear_period( s_sn, mn_index, nu_clear_period );
}

void WndChartTrend::slot_btn_show_all_trend()
{
    set_axis_y_show_all();
    cal_axis_y_span();
    update_axis_y();
}

void WndChartTrend::slot_btn_copy_trend()
{
    copy_chart();
}

void WndChartTrend::slot_btn_copy_temp()
{
    QString s_sn = get_sn();
    if ( s_sn.isEmpty() ) return;

    QList< ChartPoints * > list_charts;
    CDataChartMode::Instance()->get_data( s_sn, mn_index, list_charts );

    QString str_copy;
    QList< ChartPoints * >::iterator it = list_charts.begin();
    for ( ; it != list_charts.end(); ++it ) {
        ChartPoints * chart_points = (ChartPoints *)(*it);
        str_copy += chart_points->get_all_temp_string() + "\r\n";
    }

    QClipboard *p_board = QApplication::clipboard();
    p_board->setText( str_copy );
}

void WndChartTrend::slot_btn_select_color()
{
    QString s_sn = get_sn();
    if ( s_sn.isEmpty() ) return;

    WndAnaChartColor* dlg_color = new WndAnaChartColor( this, mn_index, 0 ) ;
    if ( dlg_color ){
        dlg_color->set_group_number( mn_group_num );
        dlg_color->set_ana_chart_color_list( m_map_chart_color );
        if ( QDialog::Accepted == dlg_color->exec() ){
            m_map_chart_color.clear();
            dlg_color->get_ana_chart_color_list( m_map_chart_color );
            set_chart_color_list();
        }
        delete dlg_color;
        dlg_color = NULL;
    }


}

void WndChartTrend::slot_load_ir_vi_picture(bool b_have, QString str_ir_path, QString str_vi_path)
{
    qDebug() << " b_have: "<< b_have;
    qDebug() << " str_ir_path: "<< str_ir_path;
    qDebug() << " str_vi_path: "<< str_vi_path;
     if ( b_have ){
        if ( ( !str_ir_path.isEmpty()) || ( !str_vi_path.isEmpty() ) ){

//            str_ir_path = "E:/MSWorking/converter-valve-client1/distribute/win32-mingw/search_ir.irp";
//            str_vi_path = "E:/MSWorking/converter-valve-client1/distribute/win32-mingw/search_vi.jpg";


            DeviceGroup  devGroup;
            CDataMode::Instance()->get_group_info( mn_group_num, devGroup );

            QString str_time = QDateTime::fromTime_t( mn_double_click_mark_time ).toString( "yyyy-MM-dd HH:mm:ss" );
            QString str_caption;
            if ( mn_index == MS_PRESET_CHART_INDEX ){
                str_caption = QString( "%1预置点:%2时间:%3红外可见光" ).arg( devGroup.str_name ).arg( m_str_preset_name ).arg( str_time );
            } else {
                str_caption = QString( "%1时间:%2红外可见光" ).arg( devGroup.str_name ).arg( str_time );
            }

            if ( mp_wnd_picture ){
                if ( str_ir_path.isEmpty() ){
                    mp_wnd_picture->set_palette_enable( false );
                }else{
                    mp_wnd_picture->set_palette_enable( true );
                }
                mp_wnd_picture->delete_all_ana();
                mp_wnd_picture->set_caption( str_caption );
                mp_wnd_picture->open_ir_vi_image( str_ir_path, str_vi_path );
                if ( !mp_wnd_picture->isVisible() ){
                    mp_wnd_picture->exec();
                }
            }
        }
    }

}


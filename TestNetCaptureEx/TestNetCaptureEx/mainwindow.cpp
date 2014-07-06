#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "YFDrawShape.h"
#include "YFFormatAnaStr.h"
#include <string>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QToolTip>
#include <qlogging.h>
#include <QDebug>
#include <QDateTime>

#include "../public/net/lib-ms-net.h"
#include "../public/net/yf_protocol_def.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    yf_capture_ex_init( mh_net_capture );

    MS_NET::net_discover_init( mh_discover );
    MS_NET::net_discover_init_as_server( mh_discover, "0.0.0.0", DiscoverServerPort, on_discover, this );
    MS_NET::net_discover_broadcast( mh_discover, DiscoverClientPort );
}

MainWindow::~MainWindow()
{
    MS_NET::net_discover_un_init( mh_discover );
    yf_capture_ex_uninit( mh_net_capture );
    delete ui;
}

bool MainWindow::on_discover( const string &s_name, const string &s_ip, void *p_user_data )
{
    MainWindow* p_this = ( MainWindow* )p_user_data;
    if( p_this ){
        return p_this->on_discover_real( s_name, s_ip );
    }
    return false;
}

void MainWindow::draw_anas()
{
    if( m_image.isNull() ) return;

    DrawStruct draw_struct;
    draw_struct.pby_bits = m_image.bits();
    draw_struct.w_bits_count = 32;
    draw_struct.w_width = get_width();
    draw_struct.w_height = get_height();
    draw_struct.by_line_width = 1;

    float ft_max, ft_min;
    unsigned short w_x, w_y;
    if( yf_capture_ex_get_max_temp( mh_net_capture, w_x, w_y, ft_max ) ){
        draw_struct.dw_line_clr = 0xFF;
        yf_draw_max_point( w_x, w_y, draw_struct );
    }

    if( yf_capture_ex_get_min_temp( mh_net_capture, w_x, w_y, ft_min ) ){
        draw_struct.dw_line_clr = 0xFF0000;
        yf_draw_min_point( w_x, w_y, draw_struct );
    }

    string str_anas;
    if( !yf_capture_ex_get_anas_str( mh_net_capture, str_anas ) ) return;

    draw_struct.dw_line_clr = 0x00FFFF00;
    yf_draw_shape( str_anas, draw_struct );

    QString qstr_anas( str_anas.c_str() );
    QStringList list_anas = qstr_anas.split( "-", QString::SkipEmptyParts );
    if( list_anas.empty() ) return;

    YFAnaType ana_type;
    for( int i = 0; i < list_anas.size(); ++i ){
        if( yf_parse_ana_char_a( list_anas[ i ].at( 0 ).toLatin1(), ana_type ) && ( ana_type == YFPoint ) ) continue;

        if( yf_capture_ex_get_ana_max_temp( mh_net_capture, list_anas[ i ].toStdString() + "-", w_x, w_y, ft_max ) ){
            draw_struct.dw_line_clr = 0xFF;
            yf_draw_max_point( w_x, w_y, draw_struct );
        }

        if( yf_capture_ex_get_ana_min_temp( mh_net_capture, list_anas[ i ].toStdString() + "-", w_x, w_y, ft_min ) ){
            draw_struct.dw_line_clr = 0xFF0000;
            yf_draw_min_point( w_x, w_y, draw_struct );
        }
    }
}

void MainWindow::draw_anas_text( QPainter *p_draw )
{
    if( m_image.isNull() ) return;

    p_draw->setPen( QColor( 255, 255, 255 ) );

    int i_text_x = 5, i_text_y = 50;
    float ft_max, ft_min;
    QString str_temp;
    unsigned short w_x, w_y;
    if( yf_capture_ex_get_max_temp( mh_net_capture, w_x, w_y, ft_max ) && yf_capture_ex_get_min_temp( mh_net_capture, w_x, w_y, ft_min ) ){
        str_temp.sprintf( "Whole Image Max: %.01f, Min: %.01f", ft_max, ft_min );
        p_draw->drawText( i_text_x, i_text_y, str_temp ); i_text_y += 20;
    }

    string str_anas;
    if( !yf_capture_ex_get_anas_str( mh_net_capture, str_anas ) ) return;

    QString qstr_anas( str_anas.c_str() );
    QStringList list_anas = qstr_anas.split( "-", QString::SkipEmptyParts );
    if( list_anas.empty() ) return;

    YFAnaType ana_type;
    for( int i = 0; i < list_anas.size(); ++i ){
        if( !yf_parse_ana_char_a( list_anas[ i ].at( 0 ).toLatin1(), ana_type ) ) continue;

        if( ana_type == YFPoint ){
            if( yf_capture_ex_get_ana_max_temp( mh_net_capture, list_anas[ i ].toStdString() + "-", w_x, w_y, ft_max ) ){
                str_temp.sprintf( "%s Temp: %.01f", list_anas[ i  ].toStdString().c_str(), ft_max );
                p_draw->drawText( i_text_x, i_text_y, str_temp ); i_text_y += 20;
            }
        }else{
            if( yf_capture_ex_get_ana_max_temp( mh_net_capture, list_anas[ i ].toStdString() + "-", w_x, w_y, ft_max )
                    && yf_capture_ex_get_ana_min_temp( mh_net_capture, list_anas[ i ].toStdString() + "-", w_x, w_y, ft_min ) ){
                str_temp.sprintf( "%s Max: %.01f, Min: %.01f", list_anas[ i  ].toStdString().c_str(), ft_max, ft_min );
                p_draw->drawText( i_text_x, i_text_y, str_temp ); i_text_y += 20;
            }
        }
    }
}

void MainWindow::draw_mac_srial( QPainter *p_draw )
{
    int i_text_x = 5, i_text_y = 20;
    string str_mac_serial;
    if( yf_capture_ex_get_mac_serial( mh_net_capture, str_mac_serial ) ){
        p_draw->drawText( i_text_x, i_text_y, QString( str_mac_serial.c_str() ) );
    }
}

bool MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip) {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
        int i_x = helpEvent->x(), i_y = helpEvent->y();
        if( i_x < get_width() && i_y < get_height() ){
            float ft_temp;
            if( yf_capture_ex_get_temp_from_loc( mh_net_capture, i_x, i_y, ft_temp ) ){
                QString str_temp; str_temp.sprintf( "[%d,%d]:%.01f", i_x, i_y, ft_temp );
                QToolTip::showText( helpEvent->globalPos(), str_temp );
            }
        } else {
            QToolTip::hideText();
            event->ignore();
        }
        return true;
    }

    return QWidget::event(event);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter draw;
    draw.begin( this );

    unsigned int dw_size;
    unsigned char sz_buff[1024*1024*2];
    //if( yf_capture_ex_get_cur_bits( mh_net_capture, m_image.bits(), dw_size ) ){
    //
    QString str = QString("%1").arg( QDateTime::currentDateTime().time().msec() );
    //qDebug() << "paintEvent" << str;

    if( m_image.isNull() ){
        draw.fillRect( 1, 1, 640, 480, QBrush( QColor( 200, 200, 200 ) ) );
    } else {
        draw_anas();
        draw.drawImage( 1, 1, m_image );
        draw_anas_text( &draw );
        draw_mac_srial( &draw );
    }

    draw.end();
}

void MainWindow::on_id_btn_connect_clicked()
{
    if ( ui->id_btn_connect->text() == "connect" ) {
        string s_ip = ui->lineEdit->text().toStdString();
        if ( yf_capture_ex_connect( mh_net_capture, s_ip ) ) {
            yf_capture_ex_transfer_data( mh_net_capture, true );
            ui->id_btn_connect->setText( "Disconnect" );
            yf_capture_ex_set_cb_bits_ready( mh_net_capture, cb_bits_ready, this );

            unsigned short w_width, w_height;
            yf_capture_ex_get_image_resolution( mh_net_capture, w_width, w_height );
            mw_width = w_width;
            mw_height = w_height;
            QString str = QString("%1 %2").arg( mw_width ).arg( mw_height );
            //qDebug() << "size" << str;
            m_image = QImage( w_width, w_height, QImage::Format_RGB32);
        }
    } else {
        QString str = QString("%1 %2").arg( mw_width ).arg( mw_height );
        //qDebug() << "size" << str;
        if ( yf_capture_ex_disconnect( mh_net_capture ) ) {
            yf_capture_ex_stop_cb_bits_ready( mh_net_capture, cb_bits_ready );
            ui->id_btn_connect->setText( "connect" );
            //yf_capture_ex_uninit( mh_net_capture );
        }
    }
}

void cb_bits_ready(void *p_user_data, YHANDLE h)
{
    //static int n = 0; n++;
    //if ( n%16 == 1 ) {
        MainWindow *p_this = ( MainWindow * )p_user_data;
        p_this->update_show( h );
   // }
}

void MainWindow::update_show( YHANDLE h )
{
    unsigned int dw_size;
    unsigned char sz_buff[1024*1024*2];
    if( yf_capture_ex_get_cur_bits( mh_net_capture, m_image.bits(), dw_size ) ){
        update( 1, 1, mw_width, mw_height );
        QString str = QString("%1 %2").arg( mw_width ).arg( mw_height );
        //qDebug() << "update_show" << str;
    }
}

void MainWindow::on_id_btn_search_clicked()
{
    MS_NET::net_discover_broadcast( mh_discover, DiscoverClientPort );
}

bool MainWindow::on_discover_real( const string &s_name, const string &s_ip )
{
    int a = 0;
    return true;
}

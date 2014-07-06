#include "pictureshow.h"
#include "ui_pictureshow.h"
#include <QFrame>
#include <QPainter>
#include <QBuffer>
#include <QHBoxLayout>
#include "picpreview.h"
#include <QDebug>

PictureShow::PictureShow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PictureShow)
    ,mp_image( NULL )
    ,mp_slider( NULL )
{
    ui->setupUi(this);
    PicPreviewShowInfo pic_pre_show_info;
    pic_pre_show_info.p_image = new QImage();
    pic_pre_show_info.str_filename = "";
    pic_pre_show_info.str_path = "";
    pic_pre_show_info.str_sn = "";
    m_list_show_frame.append( pic_pre_show_info );
    ui->id_widget->set_view( ui->id_frame, ui->id_move_frame, ui->id_frame->get_select_image_name(), &m_list_show_frame );

}

PictureShow::~PictureShow()
{
    delete ui;

}

void PictureShow::paintEvent( QPaintEvent *event )
{

    //QPainter painter( this );
    //标题栏背景图片
    //painter.drawPixmap( 0, 0, 100, 100, QPixmap( "../logo.png" ) );

    QPainter draw;
    draw.begin( this );

    int n_size = m_list_show_frame.size();
    QList<PicPreviewShowInfo>::iterator i;
    int j=0;
    for( i=m_list_show_frame.begin(), j=0; i!=m_list_show_frame.end(), j<n_size; i++,j++ ) {
        /*if( ms_image_name == i->str_filename ) {
            draw.fillRect( ( IMAGE_WIDTH+SPACE )*j+SPACE-mn_len, SPACE, IMAGE_WIDTH+5, height()-SPACE , QBrush( QColor( 200, 200, 200 ) ));
        }*/

        if( i->p_image->isNull() ){
            //draw.fillRect( ui->id_move_frame->x(), ui->id_move_frame->y(), ui->id_move_frame->width(), ui->id_move_frame->height() , QBrush( QColor( 200, 200, 200 ) ));
        } else {
            draw.drawImage( QRect( ui->id_move_frame->x(), ui->id_move_frame->y(), ui->id_move_frame->width(), ui->id_move_frame->height() ), *( i->p_image ) );
            //draw.drawText( QRect( ( IMAGE_WIDTH+SPACE )*j+SPACE-mn_len, SPACE+height()-60, IMAGE_WIDTH, SPACE ), Qt::AlignCenter, i->str_filename );
        }
    }

    /*qDebug()<<"size"<<n_size;
    if( n_size*( IMAGE_WIDTH+SPACE ) +SPACE <=width() ) {
        mp_scrollBar->hide();
        mn_len =0;
    } else {
        mp_scrollBar->show();
        //设置滑动条控件的最大值
        mp_scrollBar->setMaximum( n_size*( IMAGE_WIDTH+SPACE )-width()+SPACE );
        mp_scrollBar->setPageStep( n_size*( IMAGE_WIDTH+SPACE )-width()+SPACE );
    }
    //draw.setPen( Qt::green ); //设定画笔颜色，到时侯就是边框颜色
    //画当前类对象的边框
    draw.drawRect( QRect( 0, 0, width()-1, height()-1 ) );
    //setStyleSheet( "border:1px solid black" );*/


    draw.end();

}

void PictureShow::resizeEvent( QResizeEvent *event )
{
    ui->id_widget->move( 0, 0 );
    ui->id_widget->resize( width(), height() );
    //ui->id_widget->setStyleSheet( "border:1px solid black" );

    ui->id_frame->move( 300, 300 );
    ui->id_frame->resize( 400, 140 );

    ui->id_move_frame->setStyleSheet( "border:1px solid black" );
    ui->id_move_frame->move( 100, 40 );
    ui->id_move_frame->resize( 200, 150 );
}

QList<PicPreviewShowInfo> *PictureShow::get_show_list_value()
{
    return &m_list_show_frame;
}

void PictureShow::setLineEditValue( int value )
{



}

#include "picpreview.h"
#include <QBuffer>
#include <QPainter>
#include <QDebug>

#define SPACE 20
#define IMAGE_WIDTH 100

PicPreview::PicPreview(QWidget *parent) :
    QFrame(parent)
    ,mn_len( 0 )
    ,mp_erase( NULL )
    ,mp_scrollBar( NULL )
    ,ms_image_name( "meishen" )
{
    mp_image = new QImage( "../logo.png" ); //, QImage::Format_RGB32 );
    QByteArray ba;
    QBuffer buffer( &ba );
    buffer.open( QIODevice::WriteOnly );
    mp_image->save( &buffer, "PNG", 0 );

    mp_image1 = new QImage( "../1.jpg" );
    mp_image1->save( &buffer, "JPG", 0 );

    mp_image2 = new QImage( "../2.jpg" );
    mp_image2->save( &buffer, "JPG", 0 );

    mp_image3 = new QImage( "../3.jpg" );
    mp_image3->save( &buffer, "JPG", 0 );

    PicPreviewInfo pic_preview_info;
    pic_preview_info.p_image = mp_image;
    pic_preview_info.str_path = "../logo.png";
    pic_preview_info.str_filename = "logo.png";
    m_list_pic_preview.append( pic_preview_info );

    pic_preview_info.p_image = mp_image1;
    pic_preview_info.str_path = "../1.jpg";
    pic_preview_info.str_filename = "1.jpg";
    m_list_pic_preview.append( pic_preview_info );

    pic_preview_info.p_image = mp_image2;
    pic_preview_info.str_path = "../2.jpg";
    pic_preview_info.str_filename = "2.jpg";
    m_list_pic_preview.append( pic_preview_info );

    pic_preview_info.p_image = mp_image3;
    pic_preview_info.str_path = "../3.jpg";
    pic_preview_info.str_filename = "3.jpg";
    m_list_pic_preview.append( pic_preview_info );

    //新建一个水平方向的滑动条QSlider控件
    mp_scrollBar = new QScrollBar( Qt::Horizontal, this );
    //设置滑动条控件的最小值
    mp_scrollBar->setMinimum( 0 );
    //设置滑动条控件的最大值
    //mp_scrollBar->setMaximum( 50 );
    //设置滑动条控件的值
    //mp_scrollBar->setValue( 100 );
    //mp_scrollBar->setPageStep( 100 );

    mp_erase = new QPushButton( this );

    //信号和槽 当滑动条的值发生改变时，即产生一个valueChanged(int)信号 设置QLineEdit控件的显示文本
    connect( mp_scrollBar, SIGNAL( valueChanged( int ) ), this, SLOT( set_line_edit_value( int ) ) );
    //connect( this, SIGNAL( clicked() ), this, SLOT( set_frame() ) );
    connect( mp_erase, SIGNAL( clicked() ), this, SLOT( delete_image() ) );

    mp_add_button = new QPushButton( this );
    connect( mp_add_button, SIGNAL( clicked() ), this, SLOT( slot_add_image() ) );
    //add_image( "../4.jpg", "0");


}

PicPreview::~PicPreview()
{
    int n_size = m_list_pic_preview.size();
    QList< PicPreviewInfo >::iterator i;
    for( i=m_list_pic_preview.begin(); i!=m_list_pic_preview.end(); i++ ) {
       if( i->p_image!=NULL ) {
           delete i->p_image;
           //m_list_pic_preview.removeAt( j );
       }
    }
    delete mp_erase;
    delete mp_add_button;
    delete mp_scrollBar;
}

void PicPreview::slot_add_image() {
    add_image( "../4.jpg", "0");
}

int PicPreview::add_image( QString str_file_path, QString str_sn )
{
    mp_image4 = new QImage( str_file_path );
    QByteArray ba;
    QBuffer buffer( &ba );
    buffer.open( QIODevice::WriteOnly );
    mp_image4->save( &buffer, "JPG", 0 );

    PicPreviewInfo pic_preview_info;
    pic_preview_info.p_image = mp_image4;
    pic_preview_info.str_path = str_file_path;
    pic_preview_info.str_filename = str_file_path.split("/")[1];

    int n_size = m_list_pic_preview.size();
    QList< PicPreviewInfo >::iterator i;
    int j=0;
    for( i=m_list_pic_preview.begin(), j=0; i!=m_list_pic_preview.end(), j<n_size; i++,j++ ) {
       if( (*i ).str_filename == pic_preview_info.str_filename ) {
            delete mp_image4;
            return 0;
       }
    }
    //m_list_pic_preview.push_back( pic_preview_info );
    m_list_pic_preview.append( pic_preview_info );
}

void PicPreview::paintEvent( QPaintEvent *event )
{
    //QPainter painter( this );
    //标题栏背景图片
    //painter.drawPixmap( 0, 0, 100, 100, QPixmap( "../logo.png" ) );

    QPainter draw;
    draw.begin( this );

    int n_size = m_list_pic_preview.size();
    QList<PicPreviewInfo>::iterator i;
    int j=0;
    for( i=m_list_pic_preview.begin(), j=0; i!=m_list_pic_preview.end(), j<n_size; i++,j++ ) {
        if( ms_image_name == (*i).str_filename ) {
            draw.fillRect( ( IMAGE_WIDTH+SPACE )*j+SPACE-mn_len, SPACE, IMAGE_WIDTH+5, height()-SPACE , QBrush( QColor( 200, 200, 200 ) ));
        }

        if( (*i).p_image->isNull() ){
            draw.fillRect( ( IMAGE_WIDTH+SPACE )*j+SPACE-mn_len, SPACE, IMAGE_WIDTH, height()-SPACE*3 , QBrush( QColor( 200, 200, 200 ) ));
        } else {
            draw.drawImage( QRect( ( IMAGE_WIDTH+SPACE )*j+SPACE-mn_len, SPACE, IMAGE_WIDTH, height()-SPACE*3 ), *( (*i).p_image ) );
            draw.drawText( QRect( ( IMAGE_WIDTH+SPACE )*j+SPACE-mn_len, SPACE+height()-60, IMAGE_WIDTH, SPACE ), Qt::AlignCenter, (*i).str_filename );
        }
    }

    qDebug()<<"size"<<n_size;
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
    //setStyleSheet( "border:1px solid black" );


    draw.end();
}

void PicPreview::resizeEvent( QResizeEvent *event )
{
    mp_scrollBar->move( 1, height()-21 );
    mp_scrollBar->resize( width()-2, 20 );

    mp_erase->move( width()-60, 0 );
    mp_erase->resize( 20, 20 );

    mp_add_button->move( width()-20, 0 );
    mp_add_button->resize( 20, 20 );
}

void PicPreview::set_line_edit_value( int value )
{
    int pos = mp_scrollBar->value();
 // QString str = QString ("%1" ).arg( pos );
  //lineEdit->setText( str );
  //mp_image->move( 20+pos*4.3, 0 );
    int length = mp_scrollBar->maximum() - mp_scrollBar->minimum() + mp_scrollBar->pageStep();
    qDebug()<<"111"<<pos<<"2222"<<length<<"3333"<<mp_scrollBar->pageStep()<<width()<<mn_len;
    mn_len = pos;

    update();

}

void PicPreview::set_frame( const QPointF *p_point ) {
    qDebug()<<"33333";
    int n_size = m_list_pic_preview.size();
    QList<PicPreviewInfo>::iterator i;
    int j=0;
    for( i=m_list_pic_preview.begin(), j=0; i!=m_list_pic_preview.end(), j<n_size; i++, j++ ) {
        //( IMAGE_WIDTH+SPACE )*j+SPACE-mn_len, SPACE, IMAGE_WIDTH, height()-SPACE*3
        if( ( p_point->x()>( IMAGE_WIDTH+SPACE )*j+SPACE-mn_len && p_point->x()<=( IMAGE_WIDTH+SPACE )*j+SPACE-mn_len +IMAGE_WIDTH )&& \
            ( p_point->y()>SPACE && p_point->y()<= SPACE+height()-SPACE*3 ) ) {
            ms_image_name = (*i).str_filename;
            update();

        }
    }

}

void PicPreview::mousePressEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton ) {
        //m_pt_press_global = event->globalPos();
        m_pt_press_local = event->localPos();
        //qDebug()<<"view event"<<event->x()<<event->y()<<"local"<<m_pt_press_local;//local==event
        mb_left_btn_press = true;
        set_frame( &m_pt_press_local );
    }
}

QList<PicPreviewInfo>* PicPreview::get_list_value( )
{
    return &m_list_pic_preview;
}

int PicPreview::get_scroll_len() {
    return mn_len;
}

bool PicPreview::get_press_status() {
    return mb_left_btn_press;
}

QString* PicPreview::get_select_image_name() {
    return &ms_image_name;
}

void PicPreview::delete_image() {
    int n_size = m_list_pic_preview.size();
    QList<PicPreviewInfo>::iterator i;
    int j=0;
    for( i=m_list_pic_preview.begin(), j=0; i!=m_list_pic_preview.end(), j<n_size; i++, j++ ) {
        if( (*i).str_filename == ms_image_name ) {
            ms_image_name = "meishen";
            //删除image
            delete (*i).p_image;
            //m_list[j] = NULL;
            m_list_pic_preview.removeAt( j );
            //remove 图片名字
            update();
            break;
        }
    }
    // qDebug()<<"list2"<<m_list<<"2222"<<mp_image1<<*mp_image1;
}

void PicPreview::delete_image( QString str_sn )
{
    int n_size = m_list_pic_preview.size();
    QList<PicPreviewInfo>::iterator i;
    int j=0;
    for( i=m_list_pic_preview.begin(), j=0; i!=m_list_pic_preview.end(), j<n_size; i++, j++ ) {
        if( (*i).str_sn == str_sn ) {
            ms_image_name = "meishen";
            //删除image
            delete (*i).p_image;
            //m_list[j] = NULL;
            m_list_pic_preview.removeAt( j );
            //remove 图片名字
            update();
            break;
        }
    }
}

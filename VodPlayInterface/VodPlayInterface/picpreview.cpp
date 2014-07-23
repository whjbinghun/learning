#include "picpreview.h"
#include <QBuffer>
#include <QPainter>
#include <QDebug>
#include <qcoreapplication.h>
#include "wndconfig.h"

#define SPACE 10
#define IMAGE_WIDTH 100
//边框与图片间隔
#define FRAME_SPACE 5

PicPreview::PicPreview(QWidget *parent) :
    QFrame(parent)
    ,mn_scroll_pos( 0 )
    ,mp_erase( NULL )
    ,mp_scrollBar( NULL )
    ,ms_image_name( "" )
    , mp_wnd_config( NULL )
    , mn_class(-1)
{

    //新建一个水平方向的滑动条QSlider控件
    mp_scrollBar = new QScrollBar( Qt::Horizontal, this );
    //设置滑动条控件的最小值
    mp_scrollBar->setMinimum( 0 );
    //设置滑动条控件的最大值
    //mp_scrollBar->setMaximum( 50 );
    //设置滑动条控件的值
    //mp_scrollBar->setValue( 100 );
    //mp_scrollBar->setPageStep( 100 );
    mp_scrollBar->hide();

    connect( mp_scrollBar, SIGNAL( valueChanged( int ) ), this, SLOT( set_line_edit_value( int ) ) );


    mp_erase = new QPushButton( this );
    mp_add_button = new QPushButton( this );

    mp_erase->hide();
    mp_add_button->hide();

    connect( mp_erase, SIGNAL( clicked() ), this, SLOT( delete_image() ) );
    connect( mp_add_button, SIGNAL( clicked() ), this, SLOT( slot_add_image() ) );
}

PicPreview::~PicPreview()
{
    clear_image();

    delete mp_erase;
    delete mp_add_button;
    delete mp_scrollBar;
}

void PicPreview::slot_add_image() {
    add_image( "../4.jpg", "0");
}

void PicPreview::slot_add_image( const PicPreviewInfo &pic_info )
{
    QMap< QString, PicPreviewInfo >::iterator it = m_map_pic_preview.find( pic_info.str_sn );
    if ( it != m_map_pic_preview.end() ) { return; }

    m_map_pic_preview.insert( pic_info.str_sn, pic_info );
    cal_scroll_resize();
}

void PicPreview::set_config_wnd( WndConfig *p_wnd, int n_class )
{
    mp_wnd_config = p_wnd;
    mn_class = n_class;
}

void PicPreview::clear_image()
{
    QMap< QString, PicPreviewInfo >::iterator it = m_map_pic_preview.begin();
    for( ; it != m_map_pic_preview.end(); ++it ) {
        QImage *p_image = it.value().p_image;
        if( p_image != NULL ) { delete p_image; }
    }

    m_map_pic_preview.clear();
}

void PicPreview::init( const QList< DeviceInfo > &list_device )
{
    clear_image();

    QList< DeviceInfo >::const_iterator it = list_device.begin();
    for ( ; it != list_device.end(); ++it ) {
        QString str_app_path = QCoreApplication::applicationDirPath();
        PicPreviewInfo pic_info;
        if ( mn_class == 1 ) {
            pic_info.str_path =  str_app_path+"/1.jpg";
        } else {
            pic_info.str_path = str_app_path+"/1.bmp";
        }
        pic_info.p_image = new QImage( pic_info.str_path );
        pic_info.str_sn = it->str_sn;
        m_map_pic_preview.insert( pic_info.str_sn, pic_info );
    }

    cal_scroll_resize();
    update();
}

int PicPreview::add_image( QString str_file_path, QString str_sn )
{
    PicPreviewInfo pic_preview_info;

    if ( !str_file_path.isEmpty() ) {
        QImage *p_image = new QImage( str_file_path );
        pic_preview_info.p_image = p_image;
        pic_preview_info.str_path = str_file_path;
        pic_preview_info.str_filename = str_file_path.split("/")[1];
    } else {
        pic_preview_info.p_image = NULL;
        pic_preview_info.str_path = "";
        pic_preview_info.str_filename = "";
    }
    pic_preview_info.str_sn = str_sn;

    m_map_pic_preview.insert( str_sn, pic_preview_info );

    cal_scroll_resize();
}

void PicPreview::paintEvent( QPaintEvent *event )
{
    QPainter draw;
    draw.begin( this );

    int n_size = m_map_pic_preview.size();
    QMap< QString, PicPreviewInfo>::iterator it = m_map_pic_preview.begin();

    qDebug()<<"width"<<width()<<height();
    for( int n = 0; it != m_map_pic_preview.end(); ++it, ++n ) {
        QString str_sn = it.key();
        PicPreviewInfo &pic_preview_info = it.value();

        int n_width_one_pic = ( height()-SPACE*6 )*4/3;                     //.单张图片所画宽度
        int n_width_one_pic_cur = n_width_one_pic +SPACE*2;          //.单张图片选中所画宽度
        int n_pos_pic = n_width_one_pic * n + SPACE*2-FRAME_SPACE-mn_scroll_pos;

        QRect rct_cur( n_pos_pic, SPACE-FRAME_SPACE, n_width_one_pic+FRAME_SPACE*2, height()-SPACE*3 );
        QRect rct_pic( ( ( height()-SPACE*6)*4/3+SPACE*2 )*n+SPACE*2-mn_scroll_pos, SPACE, ( height()-SPACE*6)*4/3, height()-SPACE*6 );

        if( !ms_image_name.isEmpty() && ms_image_name == str_sn ) {
            draw.fillRect( rct_cur, QBrush( QColor( 193, 220, 252 ) ) );
            draw.setPen( QColor( 125, 162, 206 ) );
            draw.drawRect( rct_cur );
            draw.setPen( Qt::black );
        }

        if( pic_preview_info.p_image==NULL || pic_preview_info.p_image->isNull() ){
            draw.fillRect( rct_pic, QBrush( QColor( 200, 200, 200 ) ));
            draw.drawText( rct_pic, Qt::AlignCenter, "没有示例图片" );
        } else {
            draw.drawImage( rct_pic, *( pic_preview_info.p_image ) );
        }
        draw.drawText( QRect( ( ( height()-SPACE*6)*4/3+SPACE*2 )*n+SPACE*2-mn_scroll_pos, SPACE+height()-SPACE*6, ( height()-SPACE*6)*4/3, SPACE*2 ), Qt::AlignCenter, str_sn );
    }

    draw.setPen( QColor( 112, 112, 112 ) );
    draw.drawRect( QRect( 0, 0, width()-1, height()-1 ) );

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

    cal_scroll_resize();
}

void PicPreview::set_line_edit_value( int value )
{
    int pos = mp_scrollBar->value();
 // QString str = QString ("%1" ).arg( pos );
  //lineEdit->setText( str );
  //mp_image->move( 20+pos*4.3, 0 );
    //int length = mp_scrollBar->maximum() - mp_scrollBar->minimum() + mp_scrollBar->pageStep();
    //qDebug()<<"111"<<pos<<"2222"<<length<<"3333"<<mp_scrollBar->pageStep()<<width()<<mn_scroll_pos;
    mn_scroll_pos = pos;

    update();
}

bool PicPreview::pt_in_image_rect( const QPointF *p_point, PicPreviewInfo &pic_info )
{
    QMap< QString, PicPreviewInfo>::iterator it = m_map_pic_preview.begin();

    for( int j=0; it != m_map_pic_preview.end(); ++it, ++j ) {
        //( IMAGE_WIDTH+SPACE )*j+SPACE-mn_scroll_pos, SPACE, IMAGE_WIDTH, height()-SPACE*3
        if( ( p_point->x()>( ( height()-SPACE*6)*4/3+SPACE*2 )*j+SPACE*2-mn_scroll_pos \
           && p_point->x()<=( ( height()-SPACE*6)*4/3+SPACE*2 )*(j+1)-mn_scroll_pos \
           && ( p_point->y()>SPACE \
           && p_point->y()<= SPACE+height()-SPACE*3 ) ) )
        {
            ms_image_name = it.key();
            pic_info = it.value();
            update();
            return true;
        }
    }

    ms_image_name = "";
    update();
    return false;
}

void PicPreview::mousePressEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton ) {
        QPointF pt_press_global = event->localPos();

        PicPreviewInfo pic_info;
        if ( pt_in_image_rect( &pt_press_global, pic_info ) && mp_wnd_config ) {
            //QPointF pt = event->windowPos();
            QPoint pt = mapTo( mp_wnd_config, pt_press_global.toPoint() );
            mp_wnd_config->mouse_press( pt, pic_info, mn_class );
        }
    }
}

void PicPreview::mouseReleaseEvent(  QMouseEvent *event  )
{
    if ( mp_wnd_config ) {
        QPoint pt = mapTo( mp_wnd_config, event->localPos().toPoint() );
        mp_wnd_config->mouse_release( pt, mn_class );
    }
}

void PicPreview::delete_image()
{
    delete_image( ms_image_name );
}

void PicPreview::delete_image( QString str_sn )
{
    QMap< QString, PicPreviewInfo>::iterator it = m_map_pic_preview.begin();
    for( ; it != m_map_pic_preview.end(); ++it ) {
        if( it.key() == str_sn ) {
            if ( ms_image_name == str_sn ) {
                ms_image_name = "";
            }

            it = m_map_pic_preview.erase( it );

            update();
            return;
        }
    }
}

int PicPreview::get_preview_width()
{
    int n_preview_count = m_map_pic_preview.size();
    return n_preview_count*( ( height()-SPACE*6)*4/3+SPACE*2 ) +SPACE*2;
}

void PicPreview::cal_scroll_resize()
{
    int n_preview_width = get_preview_width();
    if( n_preview_width <= width() ) {
        mp_scrollBar->hide();
        mn_scroll_pos =0;
    } else {
        mp_scrollBar->setMaximum( n_preview_width - width() );
        mp_scrollBar->setPageStep( n_preview_width - width() );
        mp_scrollBar->show();
    }
}

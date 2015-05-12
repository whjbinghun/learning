#include "deletelabel.h"
#include <QPainter>

DeleteLabel::DeleteLabel(QWidget *parent) :
    QLabel(parent)
    ,mb_delete_status( false )
{
    /*QString path=":res/delete_btn.jpg";//相对项目文件
    QPixmap img( path );
    setPixmap( img );
    */
}

DeleteLabel::~DeleteLabel()
{

}

void DeleteLabel::resizeEvent( QResizeEvent *event )
{

}

void DeleteLabel::paintEvent( QPaintEvent *event )
{
    QPainter draw;
    draw.begin( this );

    if( mb_delete_status ) {
        draw.fillRect( 0, 0, width(), height(), QBrush( QColor( 177, 1, 1 ) ) );
    } else {
        draw.fillRect( 0, 0, width(), height(), QBrush( QColor(166,166,166) ) );
    }
    draw.drawPixmap( 0, 0, width(), height(), QPixmap( ":image/image/delete.png" ) );

    draw.drawRect( 0, 0, width()-1, height()-1 );

    draw.end();
}

void DeleteLabel::mousePressEvent( QMouseEvent *event )
{

}

void DeleteLabel::mouseMoveEvent( QMouseEvent *event )
{

}

void DeleteLabel::mouseReleaseEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton ) {
        mb_delete_status = true;
        emit sig_delete_shape();
        update();
    }
}

bool DeleteLabel::get_delete_status()
{
    return mb_delete_status;
}

void DeleteLabel::set_delete_status( bool b_delete_status )
{
    mb_delete_status = b_delete_status;
    update();
}

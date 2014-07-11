#include "deletelabel.h"
#include <QPainter>
#include "ana_label.h"

DeleteLabel::DeleteLabel(QWidget *parent) :
    QLabel(parent)
    ,mb_delete_status( false )
    ,mp_ana_label( NULL )
{
    /*QString path=":res/delete_btn.jpg";//相对项目文件
    QPixmap img( path );
    setPixmap( img );
    */
    setText( "删除" );
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
    }
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
        mp_ana_label->set_mouse_press_status( Ana_Label::none_press_status );
        mp_ana_label->update();
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
}

void DeleteLabel::set_ana_label(Ana_Label *p_ana_label)
{
    mp_ana_label = p_ana_label;
}


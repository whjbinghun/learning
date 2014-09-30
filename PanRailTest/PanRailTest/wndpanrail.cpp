#include "wndpanrail.h"
#include <QDebug>

#define PAN_WIDTH 220
#define PAN_HEIGHT 160
#define SPACE 20

WndPanRail::WndPanRail(QWidget *parent) :
    QWidget(parent)
    ,mp_pan_rail_widget( NULL )
    ,mp_pan_widget( NULL )
    ,mb_rail_hidden( true )
    ,mb_pan_hidden( true )
    ,mb_rail_left_right_ver( true )
    ,mb_rail_lefttop_rightbottom_hor( true )
{
    mp_pan_rail_widget = new QWidget( this );
    mp_pan_widget = new QWidget( this );
}

WndPanRail::~WndPanRail()
{
    delete mp_pan_widget;

}


void WndPanRail::resizeEvent( QResizeEvent *event )
{
    if( mb_rail_hidden ) {
        mp_pan_rail_widget->move( (width()-PAN_WIDTH)/2, PAN_HEIGHT+SPACE );
        mp_pan_rail_widget->resize( PAN_WIDTH, PAN_HEIGHT );
        mp_pan_widget->move( (width()-PAN_WIDTH)/2, PAN_HEIGHT+SPACE );
        mp_pan_widget->resize( PAN_WIDTH, PAN_HEIGHT );
        move_pan_btn();
    }
    qDebug()<<"WndPanRail::resizeEvent"<<width()<<height();
}

void WndPanRail::move_pan_btn()
{

}

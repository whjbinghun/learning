#include "irslider.h"

#define POS_VALUE 6

IrSlider::IrSlider(QWidget *parent) :
    QSlider(parent)
    ,mn_pos( 0 )
    ,mb_value_change_status( false )
{
    this->setOrientation( Qt::Horizontal );
}

IrSlider::~IrSlider()
{

}

void IrSlider::mousePressEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton && this->isEnabled() ) {
        mb_value_change_status = true;
        int dur = maximum() - minimum();
        mn_pos = minimum() + dur * ( (double)event->x() / width() );
        if( mn_pos != this->sliderPosition() ) {
            setValue( mn_pos );
            emit signal_value_change( mn_pos );
        }
    }
}

void IrSlider::mouseMoveEvent( QMouseEvent *event )
{
    /*
        int dur = maximum() - minimum();
        mn_pos = minimum() + dur * ( (double)event->x() / width() );
        if( mn_pos != this->sliderPosition() ) {
            setValue( mn_pos );
        }
    */

}

void IrSlider::mouseReleaseEvent( QMouseEvent *event )
{
    if( event->button() == Qt::LeftButton && this->isEnabled() ) {
        mb_value_change_status = false;
    }
}

void IrSlider::slot_slider_value()
{
    int pos = mn_pos+POS_VALUE;
    if( pos != this->sliderPosition() ) {
        setValue( pos );
        mn_pos = pos;
    }
}

int IrSlider::get_value_pos( ) {
    return mn_pos;
}

void IrSlider::set_value_pos( int n_pos )
{
    mn_pos = n_pos;
}

bool IrSlider::get_value_status(){
    return mb_value_change_status;
}

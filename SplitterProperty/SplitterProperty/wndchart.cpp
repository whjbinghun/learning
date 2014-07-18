#include "wndchart.h"
#include <QPushButton>

WndChart::WndChart(QWidget *parent) :
    QWidget(parent)
    ,mp_button( NULL )
{
    mp_button = new QPushButton( tr( "Press me" ), this );
    mp_button->show();
}

void WndChart::resizeEvent( QResizeEvent *event )
{
    int n_width = width()/2;
    int n_height = height()/2;
    mp_button->move( n_width, n_height );
}

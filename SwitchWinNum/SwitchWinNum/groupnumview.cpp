#include "groupnumview.h"
#include "switchwinnum.h"
#include <QDebug>

GroupNumView::GroupNumView(QWidget *parent) :
    QListView(parent)
    ,mp_switchwin( NULL )
    ,mp_lab_move( NULL )
    ,ms_group_name( "" )
    ,mb_press( false )
{
    connect( this, SIGNAL( pressed( QModelIndex ) ), this, SLOT( slot_pressed( QModelIndex ) ) );
}

void GroupNumView::set_wnd( QLabel *p_lab, SwitchWinNum *p_switch_win )
{
    mp_switchwin = p_switch_win;
    mp_lab_move = p_lab;
}

void GroupNumView::set_press( bool b_press )
{
    mb_press = b_press;
}

bool GroupNumView::get_press()
{
    return mb_press;
}

void GroupNumView::mouseMoveEvent( QMouseEvent *event )
{
    QPoint pt = mapTo( mp_switchwin, event->localPos().toPoint() );
    mp_lab_move->move( pt );
}

void GroupNumView::mouseReleaseEvent( QMouseEvent *event )
{
    if ( mp_switchwin ) {
        QPoint pt = mapTo( mp_switchwin, event->localPos().toPoint() );
        mp_switchwin->mouse_release( pt, event->globalX(), event->globalY(), ms_group_name  );
    }
}

void GroupNumView::slot_pressed( QModelIndex model_index )
{
    ms_group_name = model_index.data().toString();
    mb_press = true;
    mp_lab_move->setText( ms_group_name );
    mp_lab_move->show();
}

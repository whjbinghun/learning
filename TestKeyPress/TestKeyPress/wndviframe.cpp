#include "wndviframe.h"

WndViFrame::WndViFrame(QWidget *parent) :
    QFrame(parent)
{
    setFocusPolicy( Qt::StrongFocus );
}

WndViFrame::~WndViFrame()
{

}

void WndViFrame::keyPressEvent( QKeyEvent *event )
{

}

#include "wndirframe.h"

WndIrFrame::WndIrFrame(QWidget *parent) :
    QFrame(parent)
{
    setFocusPolicy( Qt::StrongFocus );
}

WndIrFrame::~WndIrFrame()
{

}

void WndIrFrame::keyPressEvent( QKeyEvent *event )
{

}

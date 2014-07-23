#include "tooltipapp.h"
#include "ui_tooltipapp.h"
#include <QToolTip>

ToolTipApp::ToolTipApp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ToolTipApp)
{
    ui->setupUi(this);
    ui->centralWidget->setMouseTracking( true );
    setMouseTracking( true );
}

ToolTipApp::~ToolTipApp()
{
    delete ui;
}

void ToolTipApp::mousePressEvent( QMouseEvent *event )
{
    /*if( event->button() == Qt::LeftButton ) {

    }*/
}

void ToolTipApp::mouseMoveEvent( QMouseEvent *event )
{
    set_tool_tile_text( event->globalPos(), QPoint( event->x(), event->y() ) );
}

void ToolTipApp::mouseReleaseEvent( QMouseEvent *event )
{

}



void ToolTipApp::set_tool_tile_text( QPoint pt_global, QPoint pt_local )
{
    QToolTip::showText( pt_global, "全局："+QString::number(pt_global.x())
                        +","+QString::number(pt_global.y() )
                        +"局部："+QString::number(pt_local.x())+","
                        +QString::number( pt_local.y() ), this );
}

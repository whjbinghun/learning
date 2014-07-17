#include "propertybrowsert.h"
#include "ui_propertybrowsert.h"
#include <QDebug>

PropertyBrowserT::PropertyBrowserT(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PropertyBrowserT)
    ,mp_dock_widget( NULL )
{
    ui->setupUi(this);
    init_dock_widget();
    ui->id_widget->resize( 800, 600 );
    setCentralWidget( ui->id_widget );

}

PropertyBrowserT::~PropertyBrowserT()
{
    delete mp_dock_widget;
    delete ui;
}

void PropertyBrowserT::resizeEvent(QResizeEvent *event)
{
    ui->id_widget->setStyleSheet( "border:1px solid black" );
    qDebug()<<"PropertyBrowserT::resizeEvent"<<ui->centralWidget->width()<<ui->centralWidget->height()<<width()<<height();
}

void PropertyBrowserT::init_dock_widget()
{
    mp_dock_widget = new PropertyDockWidget( this );
    addDockWidget( Qt::RightDockWidgetArea, mp_dock_widget );
    //浮动框：不可关闭、不可移动、不可浮动
    mp_dock_widget->setFeatures( QDockWidget::NoDockWidgetFeatures );
}


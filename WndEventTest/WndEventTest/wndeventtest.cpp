#include "wndeventtest.h"
#include "ui_wndeventtest.h"
#include <QDebug>
#include <QPainter>

WndEventTest::WndEventTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WndEventTest)
{
    ui->setupUi(this);
    setMouseTracking(true);
    ui->id_widget_2->init_ctrl( this );
}

WndEventTest::~WndEventTest()
{
    delete ui;
}

bool WndEventTest::event( QEvent *p_event )
{
    if ( p_event->type()==QEvent::MouseButtonPress ) {
        QMouseEvent *p_mouse_event = (QMouseEvent *)p_event;
        if ( p_mouse_event && p_mouse_event->button() == Qt::LeftButton ) {
            qDebug()<<"WndEventTest::event"<<endl;
        }
    } else if( p_event->type() == QEvent::Paint ) {
        qDebug()<<"WndEventTest::paint"<<endl;
    }

    QWidget::event( p_event );//调用父类event
}

void WndEventTest::mousePressEvent( QMouseEvent *p_event )
{
    if( p_event->button() == Qt::LeftButton ) {
        qDebug()<<"WndEventTest::mousePressEvent"<<endl;
    }
}

void WndEventTest::paintEvent( QPaintEvent *p_event )
{
    QPainter draw;
    draw.begin( this );

    draw.end();
}


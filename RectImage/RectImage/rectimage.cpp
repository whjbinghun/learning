#include "rectimage.h"
#include "ui_rectimage.h"
#include <QPainter>

RectImage::RectImage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RectImage)
{
    ui->setupUi(this);
    QRectF target(10.0, 20.0, 80.0, 60.0);
    QRectF source(0.0, 0.0, 640, 480);
    //QImage image(":/images/myImage.png");

    m_image = QImage( 640, 480, QImage::Format_RGB32);

    //QPainter painter(this);
    //painter.drawImage(target, m_image, source);
}

RectImage::~RectImage()
{
    delete ui;
}

void RectImage::paintEvent(QPaintEvent * event) {
    QPainter draw;
    draw.begin( this );

    //红外显示
    if( m_image.isNull() ) {
        draw.drawImage( 0, 0, m_image );
    } else {
        //draw.drawImage( 0, 0, m_image );
        draw.drawImage(QRect(191,185,ui->id_frame->width(),ui->id_frame->height()), m_image);
        //draw.drawImage( QPoint(100,100), m_image );
        //使用ui->centralWidget->rect()可以使得窗口是可变化的
        //draw.drawImage( ui->centralWidget->rect(), m_image, QRect(0, 0, 640, 480) );
    }
    draw.end();
}

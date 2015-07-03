#include "wndimage.h"
#include "ui_wndimage.h"
#include <QPainter>
#include <QDir>

WndImage::WndImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WndImage)
{
    ui->setupUi(this);
}

WndImage::~WndImage()
{
    delete ui;
}

void WndImage::paintEvent(QPaintEvent *event)
{
    QPainter draw;
    draw.begin( this );
    QString str_path =  QDir::currentPath()+"/8254.jpg";
    draw.drawPixmap( 0, 0, QPixmap( str_path ));
    draw.end();
}

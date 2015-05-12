#include "wndimage.h"
#include "ui_wndimage.h"

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

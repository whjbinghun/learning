#include "wndanaattrubite.h"
#include "ui_wndanaattrubite.h"

WndAnaAttrubite::WndAnaAttrubite(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WndAnaAttrubite)
{
    ui->setupUi(this);
}

WndAnaAttrubite::~WndAnaAttrubite()
{
    delete ui;
}

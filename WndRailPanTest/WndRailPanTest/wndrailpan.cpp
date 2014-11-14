#include "wndrailpan.h"
#include "ui_wndrailpan.h"

WndRailPan::WndRailPan(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WndRailPan)
{
    ui->setupUi(this);
}

WndRailPan::~WndRailPan()
{
    delete ui;
}

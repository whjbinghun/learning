#include "caldener.h"
#include "ui_caldener.h"

Caldener::Caldener(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Caldener)
{
    ui->setupUi(this);
}

Caldener::~Caldener()
{
    delete ui;
}

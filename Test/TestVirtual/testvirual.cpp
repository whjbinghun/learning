#include "testvirual.h"
#include "ui_testvirual.h"

TestVirual::TestVirual(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestVirual)
{
    ui->setupUi(this);
}

TestVirual::~TestVirual()
{
    delete ui;
}

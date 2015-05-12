#include "testgroupbox.h"
#include "ui_testgroupbox.h"

TestGroupBox::TestGroupBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestGroupBox)
{
    ui->setupUi(this);
}

TestGroupBox::~TestGroupBox()
{
    delete ui;
}

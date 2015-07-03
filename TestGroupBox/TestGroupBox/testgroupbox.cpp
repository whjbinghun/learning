#include "testgroupbox.h"
#include "ui_testgroupbox.h"
#include <QLineEdit>

TestGroupBox::TestGroupBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestGroupBox)
{
    ui->setupUi(this);
    ui->groupBox->setStyleSheet("QLineEdit#mp_line_edit{background-color: yellow }");
}

TestGroupBox::~TestGroupBox()
{
    delete ui;
}

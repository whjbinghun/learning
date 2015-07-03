#include "teststylesheet.h"
#include "ui_teststylesheet.h"

TestStyleSheet::TestStyleSheet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestStyleSheet)
{
    ui->setupUi(this);

}

TestStyleSheet::~TestStyleSheet()
{
    delete ui;
}

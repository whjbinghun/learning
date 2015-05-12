#include "wndeventtest.h"
#include "ui_wndeventtest.h"

WndEventTest::WndEventTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WndEventTest)
{
    ui->setupUi(this);
}

WndEventTest::~WndEventTest()
{
    delete ui;
}

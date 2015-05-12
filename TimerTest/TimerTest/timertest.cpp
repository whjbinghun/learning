#include "timertest.h"
#include "ui_timertest.h"

TimerTest::TimerTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TimerTest)
{
    ui->setupUi(this);
}

TimerTest::~TimerTest()
{
    delete ui;
}

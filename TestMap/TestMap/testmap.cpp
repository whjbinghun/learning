#include "testmap.h"
#include "ui_testmap.h"

TestMap::TestMap(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestMap)
{
    ui->setupUi(this);
}

TestMap::~TestMap()
{
    delete ui;
}

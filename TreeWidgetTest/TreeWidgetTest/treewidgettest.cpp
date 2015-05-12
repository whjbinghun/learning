#include "treewidgettest.h"
#include "ui_treewidgettest.h"

TreeWidgetTest::TreeWidgetTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TreeWidgetTest)
{
    ui->setupUi(this);
}

TreeWidgetTest::~TreeWidgetTest()
{
    delete ui;
}

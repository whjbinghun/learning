#include "WidgetTest.h"
#include "ui_WidgetTest.h"
#include "FirstDialog.h"

WidgetTest::WidgetTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WidgetTest)
{
    ui->setupUi(this);
    setWindowFlags( Qt::FramelessWindowHint );
    connect( ui->pushButton, SIGNAL(clicked()), this, SLOT( slot_showFirstDialog() ) );
}

WidgetTest::~WidgetTest()
{
    delete ui;
}

void WidgetTest::slot_showFirstDialog()
{
    FirstDialog *p = new FirstDialog;
    p->show();
}

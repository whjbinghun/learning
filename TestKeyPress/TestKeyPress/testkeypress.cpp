#include "testkeypress.h"
#include "ui_testkeypress.h"

TestKeyPress::TestKeyPress(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestKeyPress)
{
    ui->setupUi(this);




}

TestKeyPress::~TestKeyPress()
{
    delete ui;
}

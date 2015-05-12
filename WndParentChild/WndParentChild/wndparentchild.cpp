#include "wndparentchild.h"
#include "ui_wndparentchild.h"

WndParentChild::WndParentChild(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WndParentChild)
{
    ui->setupUi(this);
}

WndParentChild::~WndParentChild()
{
    delete ui;
}

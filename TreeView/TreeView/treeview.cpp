#include "treeview.h"
#include "ui_treeview.h"

TreeView::TreeView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TreeView)
{
    ui->setupUi(this);
}

TreeView::~TreeView()
{
    delete ui;
}

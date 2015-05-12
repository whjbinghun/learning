#include "passwdmodify.h"
#include "ui_passwdmodify.h"

PasswdModify::PasswdModify(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswdModify)
{
    ui->setupUi(this);
}

PasswdModify::~PasswdModify()
{
    delete ui;
}

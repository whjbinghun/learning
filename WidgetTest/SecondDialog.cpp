#include "SecondDialog.h"
#include "ui_SecondDialog.h"

SecondDialog::SecondDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondDialog)
{
    ui->setupUi(this);

    setWindowFlags( Qt::FramelessWindowHint | Qt::Tool );
}

SecondDialog::~SecondDialog()
{
    delete ui;
}

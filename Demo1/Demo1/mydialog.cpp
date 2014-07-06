#include "mydialog.h"
#include "ui_mydialog.h"

MyDialog::MyDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::MyDialog)
{
    ui->setupUi(this);

}

MyDialog::~MyDialog(){
    delete ui;
}

void MyDialog::on_pushButton_clicked()
{
    //验证帐号和密码

    //1.验证成功，跳转到主窗口
    accept();
    //2.验证失败，报错，不跳转

}

void MyDialog::on_pushButton_2_clicked(){
    close();
}

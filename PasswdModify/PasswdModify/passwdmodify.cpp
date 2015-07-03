#include "passwdmodify.h"
#include "ui_passwdmodify.h"

PasswdModify::PasswdModify(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswdModify)
{
    ui->setupUi(this);
    init_connect();
}

PasswdModify::~PasswdModify()
{
    delete ui;
}

void PasswdModify::init_connect()
{
    connect( ui->id_btn_sure, SIGNAL( clicked() ), this, SLOT( slot_sure() ) );
    connect( ui->id_btn_cancel, SIGNAL( clicked() ), this, SLOT( slot_cancel() ) );
}

void PasswdModify::slot_sure()
{
    QString str_new_passwd = ui->id_edit_new_passwd->text();
    if( str_new_passwd == "" ) {
       ui->id_lab_tip->setText( "密码设置不能为空！" );
       return;
    }
    int n_compare = str_new_passwd.compare( ui->id_edit_passwd_confirm->text() );
    if( n_compare == 0 ) {
        ui->id_lab_tip->setText( "" );
        accept();
    } else {
        ui->id_lab_tip->setText( "密码不同，请重新设置！" );
        return;
    }

}

void PasswdModify::slot_cancel()
{
    ui->id_edit_new_passwd->setText( "" );
    ui->id_edit_passwd_confirm->setText( "" );
    ui->id_lab_tip->setText( "" );
    close();
}

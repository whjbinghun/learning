#include "adduser.h"
#include "ui_adduser.h"

AddUser::AddUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUser)
{
    ui->setupUi(this);
    init_user();
    init_connect();
}

AddUser::~AddUser()
{
    delete ui;
}

void AddUser::init_user()
{
    //超级管理员，管理员，操作员和普通用户
    ui->id_combox_permission_type->insertItem( 0, "超级管理员" );
    ui->id_combox_permission_type->insertItem( 1, "管理员" );
    ui->id_combox_permission_type->insertItem( 2, "操作员" );
    ui->id_combox_permission_type->insertItem( 3, "普通用户" );
}

void AddUser::init_connect()
{
    connect( ui->id_btn_sure, SIGNAL( clicked() ), this, SLOT( slot_click_sure() ) );
    connect( ui->id_btn_cancel, SIGNAL( clicked() ), this, SLOT( slot_click_cancel() ) );
}

//密码对比
void AddUser::passwd_diff()
{
    //当在ui->id_edit_confirm_passwd点击enter或者点击确认键后，需要对密码进行对比，如果不一致，需要重新输入密码
}

void AddUser::slot_click_sure()
{
    QString str_name = ui->id_edit_user_name->text();
    QString str_passwd = ui->id_edit_passwd->text();
    passwd_diff();
    QString str_permission = ui->id_combox_permission_type->currentText();
    bool b_passwd_diff = true;
    if( b_passwd_diff ) {
        close();
    }
}

void AddUser::slot_click_cancel()
{
    close();
}

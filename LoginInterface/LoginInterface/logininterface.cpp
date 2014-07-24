#include "logininterface.h"
#include "ui_logininterface.h"

LoginInterface::LoginInterface(QString str_ip, QString str_port, QString str_name, QString str_pwd, QWidget *parent ) :
    QMainWindow(parent),
    ui(new Ui::LoginInterface)
{
    ui->setupUi(this);
    ms_ip = str_ip;
    ms_port = str_port;
    ms_name = str_name;
    ms_pwd = str_pwd;
    /*model = new QSqlTableModel(this);
    model->setTable("student");
    model->setFilter(tr("id = '%1'").arg(name));
    model->select();
    ui->label->setText(tr("%1,欢迎您！ 您的信息如下：").arg(name));
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    */
}

LoginInterface::~LoginInterface()
{
    delete ui;
}

#ifndef LOGININTERFACE_H
#define LOGININTERFACE_H

#include <QMainWindow>

namespace Ui {
class LoginInterface;
}

class LoginInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginInterface( QString str_ip, QString str_port, QString str_name, QString str_pwd, QWidget *parent = 0 );
    ~LoginInterface();

private:
    Ui::LoginInterface *ui;
    QString ms_ip;
    QString ms_port;
    QString ms_name;
    QString ms_pwd;
    //QSqlTableModel *model;
};

#endif // LOGININTERFACE_H

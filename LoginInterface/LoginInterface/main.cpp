#include "logininterface.h"
#include <QApplication>
#include <QTextCodec>
#include "logindialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale( QTextCodec::codecForName( "UTF8" ) );
    LoginDialog l;
    QString str_IP;
    QString str_port;
    QString str_name;
    QString str_pwd;
    if(l.exec()==QDialog::Accepted)
    {
        str_IP = l.get_IP();
        str_port = l.get_port();
        str_name = l.get_name();
        str_pwd = l.get_pwd();
        LoginInterface w( str_IP, str_port, str_name, str_pwd );
        w.show();
        return a.exec();
    }else
    {
        return 0;
    }
}

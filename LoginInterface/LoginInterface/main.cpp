#include "logininterface.h"
#include <QApplication>
#include <QTextCodec>
//#include <QSqlDatabase>
//#include <QSqlError>
#include "logindialog.h"

/*static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("student");
    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection./n"
                     "This example needs SQLite support. Please read "
                     "the Qt SQL driver documentation for information how "
                     "to build it./n/n"
                     "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    return true;
}*/

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

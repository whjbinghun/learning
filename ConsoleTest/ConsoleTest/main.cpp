#include <QCoreApplication>
#include<QTextStream>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

   QTextStream in(stdin);
   QTextStream out(stdout);
   QString str_ip;
   QString str_port;
   QString str_name;
   QString str_passwd;
   qDebug()<<"login:";
   qDebug()<<"-ip：";
   in>>str_ip;
   qDebug()<<"-port:";
   in>>str_port;
   qDebug()<<"-name:";
   in>>str_name;
   qDebug()<<"-password：";
   in>>str_passwd;
   qDebug()<<"输出："<<str_ip<<str_port<<str_name<<str_passwd;

   qDebug()<<""




   return a.exec();
}

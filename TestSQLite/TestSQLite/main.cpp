#include "testsqlite.h"
#include <QApplication>
#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>
#include <qDebug>
#include <QString>
#include <QVector>
#include <QList>
#include <QFile>
#include <QSet>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestSQLite w;
    w.show();

    return a.exec();
}

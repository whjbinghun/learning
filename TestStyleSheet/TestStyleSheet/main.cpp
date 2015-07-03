#include "teststylesheet.h"
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDir dir;
    QString path = dir.currentPath();
    path.append("/main.css");
    QFile file(path);
    qDebug() << "main.css:" << file.fileName();

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        a.setStyleSheet(stream.readAll());
    }

    TestStyleSheet w;
    w.show();

    return a.exec();
}

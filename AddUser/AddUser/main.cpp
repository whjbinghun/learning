#include "adduser.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AddUser w;
    w.show();

    return a.exec();
}

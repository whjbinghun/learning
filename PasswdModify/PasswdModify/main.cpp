#include "passwdmodify.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PasswdModify w;
    w.show();

    return a.exec();
}

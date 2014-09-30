#include "backplay.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BackPlay w;
    w.show();

    return a.exec();
}

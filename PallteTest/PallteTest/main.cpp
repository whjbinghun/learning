#include "palltetest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PallteTest w;
    w.show();

    return a.exec();
}

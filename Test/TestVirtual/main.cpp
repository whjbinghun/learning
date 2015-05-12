#include "testvirual.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestVirual w;
    w.show();

    return a.exec();
}

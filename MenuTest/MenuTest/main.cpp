#include "menutest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MenuTest w;
    w.show();

    return a.exec();
}

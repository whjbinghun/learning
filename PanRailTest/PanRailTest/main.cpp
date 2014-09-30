#include "panrailtest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PanRailTest w;
    w.show();

    return a.exec();
}

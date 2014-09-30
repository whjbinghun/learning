#include "panrail.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PanRail w;
    w.show();

    return a.exec();
}

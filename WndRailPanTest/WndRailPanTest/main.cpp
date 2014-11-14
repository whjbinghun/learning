#include "wndrailpan.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WndRailPan w;
    w.show();

    return a.exec();
}

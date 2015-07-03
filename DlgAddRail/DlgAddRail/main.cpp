#include "dlgaddrail.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DlgAddRail w;
    w.show();

    return a.exec();
}

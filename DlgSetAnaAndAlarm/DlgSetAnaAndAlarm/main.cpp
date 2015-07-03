#include "dlgsetanaandalarm.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DlgSetAnaAndAlarm w;
    w.show();

    return a.exec();
}

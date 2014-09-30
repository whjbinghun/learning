#include "switchwinnum.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SwitchWinNum w;
    w.show();

    return a.exec();
}

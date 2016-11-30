#include "SystemTimeChange.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SystemTimeChange w;
    w.show();

    return a.exec();
}

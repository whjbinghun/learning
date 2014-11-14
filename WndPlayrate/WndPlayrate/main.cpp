#include "wndplayrate.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WndPlayrate w;
    w.show();

    return a.exec();
}

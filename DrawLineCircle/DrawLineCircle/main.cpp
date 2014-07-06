#include "drawlinecircle.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DrawLineCircle w;
    w.show();

    return a.exec();
}

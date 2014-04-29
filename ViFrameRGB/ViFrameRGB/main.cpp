#include "viframergb.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ViFrameRGB w;
    w.show();

    return a.exec();
}

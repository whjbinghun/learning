#include "testpixmap.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestPixmap w;
    w.show();

    return a.exec();
}

#include "drawtitle.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DrawTitle w;
    w.show();

    return a.exec();
}

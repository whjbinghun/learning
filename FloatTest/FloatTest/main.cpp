#include "floattest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FloatTest w;
    w.show();

    return a.exec();
}

#include "testmap.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestMap w;
    w.show();

    return a.exec();
}

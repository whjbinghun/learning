#include "caldener.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Caldener w;
    w.show();

    return a.exec();
}

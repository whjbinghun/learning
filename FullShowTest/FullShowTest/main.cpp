#include "fullshowtest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FullShowTest w;
    w.show();

    return a.exec();
}

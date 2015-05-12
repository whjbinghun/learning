#include "wndeventtest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WndEventTest w;
    w.show();

    return a.exec();
}

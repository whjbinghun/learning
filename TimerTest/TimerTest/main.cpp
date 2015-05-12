#include "timertest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TimerTest w;
    w.show();

    return a.exec();
}

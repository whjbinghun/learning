#include "teststylesheet.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestStyleSheet w;
    w.show();

    return a.exec();
}

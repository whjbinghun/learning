#include "CashBoxTest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CashBoxTest w;
    w.show();

    return a.exec();
}

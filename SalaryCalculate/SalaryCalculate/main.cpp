#include "salarycalculate.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SalaryCalculate w;
    w.show();

    return a.exec();
}

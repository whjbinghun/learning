#include "CategoryWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CategoryWidget w;
    w.show();

    return a.exec();
}

#include "propertybrowsert.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PropertyBrowserT w;
    w.show();

    return a.exec();
}

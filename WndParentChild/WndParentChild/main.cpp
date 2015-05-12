#include "wndparentchild.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WndParentChild w;
    w.show();

    return a.exec();
}

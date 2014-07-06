#include "listview.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ListView w;
    w.show();

    return a.exec();
}

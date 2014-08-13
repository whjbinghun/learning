#include "md5test.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Md5Test w;
    w.show();

    return a.exec();
}

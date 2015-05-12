#include "testgroupbox.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestGroupBox w;
    w.show();

    return a.exec();
}

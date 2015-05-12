#include "testkeypress.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestKeyPress w;
    w.show();

    return a.exec();
}

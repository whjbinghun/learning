#include "iranavifocuse.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IrAnaViFocuse w;
    w.show();

    return a.exec();
}

#include "wndcruisearea.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WndCruiseArea w;
    w.show();

    return a.exec();
}

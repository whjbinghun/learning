#include "dialogtitlebar.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DialogTitleBar w;
    w.show();

    return a.exec();
}

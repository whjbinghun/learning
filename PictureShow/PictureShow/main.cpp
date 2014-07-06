#include "pictureshow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PictureShow w;
    w.show();

    return a.exec();
}

#include "alarmingpicturepreview.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AlarmingPicturePreview w;
    w.show();

    return a.exec();
}

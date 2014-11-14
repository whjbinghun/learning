#include "addpresetdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AddPresetDialog w;
    w.show();

    return a.exec();
}

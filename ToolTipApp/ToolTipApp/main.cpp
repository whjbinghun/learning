#include "tooltipapp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ToolTipApp w;
    w.show();

    return a.exec();
}

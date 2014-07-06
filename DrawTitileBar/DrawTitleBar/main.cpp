#include "drawtitlebar.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DrawTitleBar w;
    w.show();

    a.connect( &w, SIGNAL(close_window()), &a, SLOT(quit()) );

    return a.exec();
}

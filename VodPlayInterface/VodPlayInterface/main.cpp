#include "vodplayinterface.h"
#include <QApplication>
#include <QDesktopWidget>
#include "log-handler.h"
VodPlayInterface * vod_ir_play_window;
int main( int argc, char *argv[] )
{
    qInstallMessageHandler( message_output );
    QApplication a( argc, argv );
    VodPlayInterface w;
    vod_ir_play_window = &w;
    w.show();
    a.connect( &w, SIGNAL( close_window() ), &a, SLOT( quit() ) );
    //a.connect( &w, SIGNAL( min_window() ), &w, SLOT( showMinimized() ) );
    //a.connect( &w, SIGNAL( max_window() ), &w, SLOT( showMaximized() ) );
    //窗口居屏幕中间
    w.move( ( QApplication::desktop()->width() - w.width())/2,
               ( QApplication::desktop()->height() - w.height())/2 );

    return a.exec();
}

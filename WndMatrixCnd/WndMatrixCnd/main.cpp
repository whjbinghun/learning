#include "wndmatrixcnd.h"
#include <QApplication>
#include "logiclayer/cbaseappmdl.h"
//#include "../common-funtion/dataconfig.h"
#include "mscclienttype.h"
#include "cbasemsg.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WndMatrixCnd w;

    MSCClientType::Instance();
    MSCClientType::Instance()->set_client_type( ClientPresetSet );
    CBaseAppMdl::Instance()->start(0,0);

    w.show();

    return a.exec();
}

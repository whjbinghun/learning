#include "tcpclient.h"
#include <QApplication>
#include "landdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TcpClient dlg_main;
    dlg_main.show();

    return a.exec();

    /*LandDialog dlg_load; //新建MyDialog类对象
    if(dlg_load.exec() == QDialog::Accepted){ //判断dialog执行结果
        TcpClient dlg_main( dlg_load.get_ip(), dlg_load.get_port() );
        dlg_main.show();         //如果是按下了“进入主界面”按钮，则显示主界面
        return a.exec(); //程序正常运行
    }
    else return 0;   //否则，退出程序*/
}

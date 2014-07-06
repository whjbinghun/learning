#include "mythread.h"
#include <QFile>
#include <QDebug>


MyThread::MyThread(QObject *parent) :
    QThread(parent)
  , mb_exit_thread(false)
{
}

void MyThread::run()
{
    //读取文件
    QString s_file_name = "../monitor.rgb";
    QFile file(s_file_name);

    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"Can't open the file!"<<endl;
    }

    quint64 n_len = 1920*1080*4;
    char buf[n_len];

    int i=0;

    while(!file.atEnd() && !mb_exit_thread ) {
        file.read(buf, n_len);
        usleep(40*1000);//40ms
        memcpy( mp_image->bits(), buf, n_len);
        i=i+1;
        qDebug()<<i;
        //发送信号
        emit signal_play_wnd();
     }

}

#include "mythread.h"

MyThread::MyThread(QObject *parent) :
    QThread(parent)
{
    //ArrayList<String> temp=new ArrayList<String>;
    //list = new QList<unsigned char*>();
}

void MyThread::set_value(unsigned char *puc_out_buf, unsigned int ui_data_len, int width, int height, int num, int den) {
    /*list.append(puc_out_buf);
    m_vi_play_message.ui_data_len = ui_data_len;
    m_vi_play_message.width = width;
    m_vi_play_message.height = height;
    m_vi_play_message.num = num;
    m_vi_play_message.den = den;*/
}

void MyThread::run(){
    memcpy(m_vi_image.bits(), mp_puc_out_buf, mn_data_len);

    //emit sig_thread_value();
    /*char *a = new char[1920*1080*4];
    memset(a, 0, sizeof(a));
    //QList<unsigned char*>::iterator i;
    int j = 0;
    for(quint64 i=0; i<sizeof(a); i++) {
        a[j] = *list[i];
        list.removeOne(list[i]);
        j++;
    }
    memcpy(m_vi_image.bits(),a ,m_vi_play_message.ui_data_len);
    emit sig_update();*/
}

void MyThread::thread_value(unsigned char *puc_out_buf, unsigned int ui_data_len)
{

}



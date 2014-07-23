#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QImage>

/*typedef struct {
    unsigned char* vi_buf;
    int len;
}VIPlayBuf;
*/

//可见光的播放信息
/*typedef struct {
    unsigned int ui_data_len;
    int width;
    int height;
    int num;
    int den;
}VIPlayMessage;*/

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);
    void stop();
    void set_value(unsigned char *puc_out_buf, unsigned int ui_data_len, int width, int height, int num, int den);
protected:
    void run();

private:
    volatile bool stopped;
signals:
    void sig_update();
    void sig_thread_value(unsigned char *puc_out_buf, unsigned int ui_data_len);
public slots:
    void thread_value(unsigned char *puc_out_buf, unsigned int ui_data_len);

private:
    QImage m_vi_image;
    //QList<struct VIPlayBuf> m_vi_list;
    //VIPlayMessage m_vi_play_message;
    unsigned char *mp_puc_out_buf;
    unsigned int mn_data_len;
};

#endif // MYTHREAD_H

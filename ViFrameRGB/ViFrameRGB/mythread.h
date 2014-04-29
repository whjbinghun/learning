#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QImage>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);

public:
    void run();
    void set_image( QImage *p_image ) { mp_image = p_image; }
    void exit_thread() { mb_exit_thread=true; }
signals:
    void signal_play_wnd();
public slots:

private:
    QImage *mp_image;
    bool mb_exit_thread;

};

#endif // MYTHREAD_H

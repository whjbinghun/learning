#ifndef THREADPLAYBACK_H
#define THREADPLAYBACK_H

#include <QThread>

class NetCaptureVod;

class ThreadPlayback : public QThread
{
    Q_OBJECT
public:
    explicit ThreadPlayback(NetCaptureVod *p_net_vod, QObject *parent = 0);

signals:

public slots:

public:
    void exit_thread(){
        mb_exit_run = true;
        exit();
    }

public:
    virtual void run();

private:
    NetCaptureVod *mp_net_vod;
    bool mb_exit_run;

};

#endif // THREADPLAYBACK_H

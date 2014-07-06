#ifndef VIFRAMERGB_H
#define VIFRAMERGB_H

#include <QMainWindow>
#include <QImage>
#include <QToolButton>
#include "mythread.h"

namespace Ui {
class ViFrameRGB;
}

class ViFrameRGB : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViFrameRGB(QWidget *parent = 0);
    ~ViFrameRGB();

    void paintEvent(QPaintEvent * event) ;

public slots:
    void slot_play_wnd();
    void timer_requset();
private:
    Ui::ViFrameRGB *ui;
    QImage m_image;
    QToolButton * mq_tool_button;
    MyThread my_thread;
};

#endif // VIFRAMERGB_H

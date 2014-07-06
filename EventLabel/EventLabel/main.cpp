#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <QString>
#include <stdio.h>
#include <QMouseEvent>

class EventLabel : public QLabel
{
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

//移动鼠标位置
void EventLabel::mouseMoveEvent(QMouseEvent *event)
{
    this->setText(QString("<center><h1>Move: (%1, %2)</h1></center>").arg(QString::number(event->x()),QString::number(event->y())));
}

//按下鼠标
void EventLabel::mousePressEvent(QMouseEvent *event)
{
    this->setText(QString("<center><h1>Press: (%1, %2)</h1></center>").arg(QString::number(event->x()),QString::number(event->y())));
}

//释放鼠标
void EventLabel::mouseReleaseEvent(QMouseEvent *event)
{
    QString msg;
    msg.sprintf("<center><h1>Release: (%d, %d)</h1></center>", event->x(), event->y());
    this->setText(msg);
}

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    EventLabel *label = new EventLabel;
    label->setWindowTitle("MouseEvent Demo");
    label->resize(600, 300);
    label->setMouseTracking(true);
    label->show();

    return a.exec();
}




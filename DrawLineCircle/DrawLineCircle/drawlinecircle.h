#ifndef DRAWLINECIRCLE_H
#define DRAWLINECIRCLE_H

#include <QMainWindow>

namespace Ui {
class DrawLineCircle;
}

class DrawLineCircle : public QMainWindow
{
    Q_OBJECT

public:
    explicit DrawLineCircle(QWidget *parent = 0);
    ~DrawLineCircle();
public:

    void resizeEvent(QResizeEvent *event);
private:
    Ui::DrawLineCircle *ui;
};

#endif // DRAWLINECIRCLE_H

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
    void set_slider();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
public slots:
    void slot_slider_value_change(int n_pos);
private:
    Ui::DrawLineCircle *ui;
};

#endif // DRAWLINECIRCLE_H

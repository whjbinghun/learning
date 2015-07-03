#ifndef WNDIMAGE_H
#define WNDIMAGE_H

#include <QWidget>

namespace Ui {
class WndImage;
}

class WndImage : public QWidget
{
    Q_OBJECT

public:
    explicit WndImage(QWidget *parent = 0);
    ~WndImage();

    void paintEvent(QPaintEvent *event);
private:
    Ui::WndImage *ui;
    QImage m_image;
};

#endif // WNDIMAGE_H

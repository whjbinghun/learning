#ifndef RECTIMAGE_H
#define RECTIMAGE_H

#include <QMainWindow>
#include <QImage>

namespace Ui {
class RectImage;
}

class RectImage : public QMainWindow
{
    Q_OBJECT

public:
    explicit RectImage(QWidget *parent = 0);
    ~RectImage();
    void paintEvent(QPaintEvent * event) ;
private:
    Ui::RectImage *ui;
    QImage m_image;
};

#endif // RECTIMAGE_H

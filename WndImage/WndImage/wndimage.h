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

private:
    Ui::WndImage *ui;
};

#endif // WNDIMAGE_H

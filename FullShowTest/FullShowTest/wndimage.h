#ifndef WNDIMAGE_H
#define WNDIMAGE_H

#include <QWidget>
#include <QPushButton>

class WndImage : public QWidget
{
    Q_OBJECT
public:
    explicit WndImage(QWidget *parent = 0);
public:
    void paintEvent( QPaintEvent *event );
    ~WndImage();
signals:
    void sig_show_normal();
public slots:
private:
    QPushButton *mp_btn_normal;
};

#endif // WNDIMAGE_H

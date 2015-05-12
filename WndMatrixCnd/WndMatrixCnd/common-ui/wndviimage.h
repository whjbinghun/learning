#ifndef WNDVIIMAGE_H
#define WNDVIIMAGE_H

#include <QWidget>

class WndViImage : public QWidget
{
    Q_OBJECT
public:
    explicit WndViImage(QWidget *parent = 0);
    ~WndViImage();
public:
    void paintEvent( QPaintEvent *p_event );

    bool set_image_path( QString str_image_path );

signals:

public slots:
private:
    QString ms_image_path;
    QImage m_image;
};

#endif // WNDVIIMAGE_H

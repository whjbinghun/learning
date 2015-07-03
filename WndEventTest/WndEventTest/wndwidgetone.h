#ifndef WNDWIDGETONE_H
#define WNDWIDGETONE_H

#include <QWidget>
#include <QMouseEvent>

class WndWidgetOne : public QWidget
{
    Q_OBJECT
public:
    explicit WndWidgetOne(QWidget *parent = 0);

    void paintEvent( QPaintEvent *p_event );
    void resizeEvent( QResizeEvent *p_event );
    void mousePressEvent( QMouseEvent *p_event );
signals:

public slots:
private:

};

#endif // WNDWIDGETONE_H

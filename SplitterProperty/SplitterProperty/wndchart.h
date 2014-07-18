#ifndef WNDCHART_H
#define WNDCHART_H

#include <QWidget>
#include <QPushButton>

class WndChart : public QWidget
{
    Q_OBJECT
public:
    explicit WndChart(QWidget *parent = 0);
public:
    void resizeEvent( QResizeEvent *event );
signals:

public slots:
private:
    QPushButton *mp_button;

};

#endif // WNDCHART_H

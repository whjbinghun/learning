#ifndef PANRAIL_H
#define PANRAIL_H

#include <QMainWindow>
#include "wndpanrail.h"

namespace Ui {
class PanRail;
}

class PanRail : public QMainWindow
{
    Q_OBJECT

public:
    explicit PanRail(QWidget *parent = 0);
    ~PanRail();
public:
    void resizeEvent( QResizeEvent *event );
    void paintEvent( QPaintEvent *event );
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::PanRail *ui;
    WndPanRail *mp_pan_rail;
};

#endif // PANRAIL_H

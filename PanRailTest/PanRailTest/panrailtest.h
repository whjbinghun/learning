#ifndef PANRAILTEST_H
#define PANRAILTEST_H

#include <QMainWindow>
#include "wndpanrail.h"

namespace Ui {
class PanRailTest;
}

class PanRailTest : public QMainWindow
{
    Q_OBJECT

public:
    explicit PanRailTest(QWidget *parent = 0);
    ~PanRailTest();
public:
    void resizeEvent( QResizeEvent *event );
private:
    Ui::PanRailTest *ui;
    WndPanRail *mp_pan_rail_widget;
};

#endif // PANRAILTEST_H

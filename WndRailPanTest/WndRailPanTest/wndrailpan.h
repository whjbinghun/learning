#ifndef WNDRAILPAN_H
#define WNDRAILPAN_H

#include <QWidget>

namespace Ui {
class WndRailPan;
}

class WndRailPan : public QWidget
{
    Q_OBJECT

public:
    explicit WndRailPan(QWidget *parent = 0);
    ~WndRailPan();

private:
    Ui::WndRailPan *ui;
};

#endif // WNDRAILPAN_H

#ifndef TESTVIRUAL_H
#define TESTVIRUAL_H

#include <QMainWindow>
#include "airplane.h"

namespace Ui {
class TestVirual;
}

class TestVirual : public QMainWindow
{
    Q_OBJECT

public:
    explicit TestVirual(QWidget *parent = 0);
    ~TestVirual();

private:
    Ui::TestVirual *ui;
};

#endif // TESTVIRUAL_H

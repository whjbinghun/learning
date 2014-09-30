#ifndef SALARYCALCULATE_H
#define SALARYCALCULATE_H

#include <QMainWindow>

namespace Ui {
class SalaryCalculate;
}

class SalaryCalculate : public QMainWindow
{
    Q_OBJECT

public:
    explicit SalaryCalculate(QWidget *parent = 0);
    ~SalaryCalculate();

private:
    Ui::SalaryCalculate *ui;
};

#endif // SALARYCALCULATE_H

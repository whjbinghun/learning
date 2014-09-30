#include "salarycalculate.h"
#include "ui_salarycalculate.h"

SalaryCalculate::SalaryCalculate(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SalaryCalculate)
{
    ui->setupUi(this);
}

SalaryCalculate::~SalaryCalculate()
{
    delete ui;
}

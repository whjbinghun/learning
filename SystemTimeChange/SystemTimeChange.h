#ifndef SYSTEMTIMECHANGE_H
#define SYSTEMTIMECHANGE_H

#include <QMainWindow>

namespace Ui {
class SystemTimeChange;
}

class SystemTimeChange : public QMainWindow
{
    Q_OBJECT

public:
    explicit SystemTimeChange(QWidget *parent = 0);
    ~SystemTimeChange();
private:
    bool setDate(int year, int mon, int day);
private:
    Ui::SystemTimeChange *ui;
};

#endif // SYSTEMTIMECHANGE_H

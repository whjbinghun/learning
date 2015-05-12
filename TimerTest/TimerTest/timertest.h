#ifndef TIMERTEST_H
#define TIMERTEST_H

#include <QMainWindow>

namespace Ui {
class TimerTest;
}

class TimerTest : public QMainWindow
{
    Q_OBJECT

public:
    explicit TimerTest(QWidget *parent = 0);
    ~TimerTest();

private:
    Ui::TimerTest *ui;
};

#endif // TIMERTEST_H

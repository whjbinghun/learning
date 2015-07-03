#ifndef TIMERTEST_H
#define TIMERTEST_H

#include <QMainWindow>

namespace Ui {
class TimerTest;
}

typedef enum _tagTest{
    btn1 = 1,
    btn2 = 2
}eTest;

class TimerTest : public QMainWindow
{
    Q_OBJECT

public:
    explicit TimerTest(QWidget *parent = 0);
    ~TimerTest();
public:
    void init_connect();
public slots:
    void slot_btn_1();
    void slot_btn_sender();
    void slot_btn_effective();
private:
    Ui::TimerTest *ui;
    QList<int> m_list_time;
    eTest me_test;
};

#endif // TIMERTEST_H

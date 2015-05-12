#ifndef WNDEVENTTEST_H
#define WNDEVENTTEST_H

#include <QWidget>

namespace Ui {
class WndEventTest;
}

class WndEventTest : public QWidget
{
    Q_OBJECT

public:
    explicit WndEventTest(QWidget *parent = 0);
    ~WndEventTest();

private:
    Ui::WndEventTest *ui;
};

#endif // WNDEVENTTEST_H

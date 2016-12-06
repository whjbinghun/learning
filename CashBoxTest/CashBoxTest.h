#ifndef CASHBOXTEST_H
#define CASHBOXTEST_H

#include <QWidget>

namespace Ui {
class CashBoxTest;
}

class CashBoxTest : public QWidget
{
    Q_OBJECT

public:
    explicit CashBoxTest(QWidget *parent = 0);
    ~CashBoxTest();

    void initCtrl();
    void initConnect();
public slots:
    void slot_openCashBox();
    void slot_openCashBox2();
private:
    int openCashBox();
    int openCashBox2();
private:
    Ui::CashBoxTest *ui;
};

#endif // CASHBOXTEST_H

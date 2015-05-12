#ifndef TESTSTYLESHEET_H
#define TESTSTYLESHEET_H

#include <QWidget>

namespace Ui {
class TestStyleSheet;
}

class TestStyleSheet : public QWidget
{
    Q_OBJECT

public:
    explicit TestStyleSheet(QWidget *parent = 0);
    ~TestStyleSheet();

private:
    Ui::TestStyleSheet *ui;
};

#endif // TESTSTYLESHEET_H

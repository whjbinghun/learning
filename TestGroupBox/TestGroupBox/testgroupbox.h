#ifndef TESTGROUPBOX_H
#define TESTGROUPBOX_H

#include <QWidget>

namespace Ui {
class TestGroupBox;
}

class TestGroupBox : public QWidget
{
    Q_OBJECT

public:
    explicit TestGroupBox(QWidget *parent = 0);
    ~TestGroupBox();

private:
    Ui::TestGroupBox *ui;
};

#endif // TESTGROUPBOX_H

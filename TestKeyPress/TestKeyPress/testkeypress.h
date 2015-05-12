#ifndef TESTKEYPRESS_H
#define TESTKEYPRESS_H

#include <QMainWindow>

namespace Ui {
class TestKeyPress;
}

class TestKeyPress : public QMainWindow
{
    Q_OBJECT

public:
    explicit TestKeyPress(QWidget *parent = 0);
    ~TestKeyPress();

private:
    Ui::TestKeyPress *ui;
};

#endif // TESTKEYPRESS_H

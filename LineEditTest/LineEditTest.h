#ifndef LINEEDITTEST_H
#define LINEEDITTEST_H

#include <QMainWindow>

namespace Ui {
class LineEditTest;
}

class LineEditTest : public QMainWindow
{
    Q_OBJECT

public:
    explicit LineEditTest(QWidget *parent = 0);
    ~LineEditTest();
private:
    void initCtrl();
    void initConnect();
private:
    Ui::LineEditTest *ui;
};

#endif // LINEEDITTEST_H

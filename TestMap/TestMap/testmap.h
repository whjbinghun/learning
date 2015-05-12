#ifndef TESTMAP_H
#define TESTMAP_H

#include <QMainWindow>

namespace Ui {
class TestMap;
}

class TestMap : public QMainWindow
{
    Q_OBJECT

public:
    explicit TestMap(QWidget *parent = 0);
    ~TestMap();

private:
    Ui::TestMap *ui;
};

#endif // TESTMAP_H

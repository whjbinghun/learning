#ifndef FULLSHOWTEST_H
#define FULLSHOWTEST_H

#include <QMainWindow>

namespace Ui {
class FullShowTest;
}

class FullShowTest : public QMainWindow
{
    Q_OBJECT

public:
    explicit FullShowTest(QWidget *parent = 0);
    ~FullShowTest();
public:
    void init_connect();
    void paintEvent( QPaintEvent *event );
public slots:
    void slot_show_full();
    void slot_show_normal();
private:
    Ui::FullShowTest *ui;
};

#endif // FULLSHOWTEST_H

#ifndef WNDEVENTTEST_H
#define WNDEVENTTEST_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class WndEventTest;
}

class WndEventTest : public QWidget
{
    Q_OBJECT

public:
    explicit WndEventTest(QWidget *parent = 0);
    ~WndEventTest();

    virtual bool event( QEvent *p_event );
    virtual void mousePressEvent( QMouseEvent *p_event );
    void paintEvent( QPaintEvent *p_event );
private:
    Ui::WndEventTest *ui;
};

#endif // WNDEVENTTEST_H

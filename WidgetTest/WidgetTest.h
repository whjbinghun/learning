#ifndef WIDGETTEST_H
#define WIDGETTEST_H

#include <QMainWindow>

namespace Ui {
class WidgetTest;
}

class WidgetTest : public QMainWindow
{
    Q_OBJECT

public:
    explicit WidgetTest(QWidget *parent = 0);
    ~WidgetTest();
private slots:
    void slot_showFirstDialog();
private:
    Ui::WidgetTest *ui;
};

#endif // WIDGETTEST_H

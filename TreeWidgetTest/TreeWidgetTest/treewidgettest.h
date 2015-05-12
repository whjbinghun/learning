#ifndef TREEWIDGETTEST_H
#define TREEWIDGETTEST_H

#include <QWidget>

namespace Ui {
class TreeWidgetTest;
}

class TreeWidgetTest : public QWidget
{
    Q_OBJECT

public:
    explicit TreeWidgetTest(QWidget *parent = 0);
    ~TreeWidgetTest();

private:
    Ui::TreeWidgetTest *ui;
};

#endif // TREEWIDGETTEST_H

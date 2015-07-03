#ifndef TREEWIDGETTEST_H
#define TREEWIDGETTEST_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QMessageBox>

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

    QTreeWidget *treeWidget;
        QHBoxLayout *hLayout;
};

#endif // TREEWIDGETTEST_H

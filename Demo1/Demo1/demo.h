#ifndef DEMO_H
#define DEMO_H

#include <QMainWindow>
#include "back_paly.h"
#include <QStandardItemModel>

namespace Ui {
class Demo;
}

class Demo : public QMainWindow
{
    Q_OBJECT

public:
    friend class Back_Paly;
    explicit Demo(QWidget *parent = 0);
    ~Demo();

public:
    void list_view();

public slots:
    void slot_play();
    void slot_back();
    void OnDoubleClickTree(const QModelIndex &index);

private slots:
    void on_listView_clicked(const QModelIndex &index);
    void on_listView_doubleClicked(const QModelIndex &index);

private:
    Ui::Demo *ui;
    Back_Paly *o_back_play;
    int n_widget_num;
    QStandardItemModel *standard_item;
};

#endif // DEMO_H

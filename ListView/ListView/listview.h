#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QListView>

namespace Ui {
class ListView;
}

class ListView : public QMainWindow
{
    Q_OBJECT

public:
    explicit ListView(QWidget *parent = 0);
    ~ListView();

private slots:
    void itemClicked(QModelIndex index);
private:
    Ui::ListView *ui;
    QStandardItemModel *standardItemModel;
    QListView *listView;
};

#endif // LISTVIEW_H

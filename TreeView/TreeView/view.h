#ifndef VIEW_H
#define VIEW_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QMouseEvent>

class View : public QTreeView
{
    Q_OBJECT
public:
    explicit View(QWidget *parent = 0);

public:
    void iterateOverItems();
    QList<QStandardItem*> returnTheItems();
    void mouseDoubleClickEvent( QMouseEvent *event );
private:
    QStandardItemModel *model;
};

#endif // VIEW_H

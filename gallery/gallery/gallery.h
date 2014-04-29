#ifndef GALLERY_H
#define GALLERY_H

#include <QMainWindow>
#include <QColumnView>
#include <QStandardItemModel>
#include <QListView>


namespace Ui {
class Gallery;
}

class Gallery : public QMainWindow
{
    Q_OBJECT

public:
    explicit Gallery(QWidget *parent = 0);
    ~Gallery();

public slots:
    void move_bar(int value);
    void change_bar(int value);

private:
    Ui::Gallery *ui;
    //QListView *listView;
    QStandardItemModel *standardItemModel;

private slots:
    void itemClicked(QModelIndex index);

};

#endif // GALLERY_H

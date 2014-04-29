#include "listview.h"
#include "ui_listview.h"
#include <QDebug>

ListView::ListView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ListView)
{
    ui->setupUi(this);
    listView = new QListView(this);
    standardItemModel = new QStandardItemModel(this);

    QStringList strList;

    strList.append("string1");
    strList.append("string2");
    strList.append("string3");
    strList.append("string4");
    strList.append("string5");
    strList.append("string6");
    strList.append("string7");
    strList << "string8";
    strList += "string9";
    int nCount = strList.size();
    qDebug()<<nCount;
    for(int i = 0; i < nCount; i++)
    {
        QString string = static_cast<QString>(strList.at(i));
        QStandardItem *item = new QStandardItem(string);
        if(i % 2 == 1)
        {
            QLinearGradient linearGrad(QPointF(0, 0), QPointF(200, 200));
            linearGrad.setColorAt(0, Qt::darkGreen);
            linearGrad.setColorAt(1, Qt::yellow);
            QBrush brush(linearGrad);
            item->setBackground(brush);
        }
        standardItemModel->appendRow(item);
    }
    listView->setModel(standardItemModel);
    listView->setFixedSize(200,300);
    connect(listView,SIGNAL(clicked(QModelIndex)),this,SLOT(itemClicked(QModelIndex)));
}

ListView::~ListView()
{
    delete ui;
}

void ListView::itemClicked(QModelIndex index)
{
    qDebug() << index.data().toString();
}

#include <QApplication>
#include <QGridLayout>
#include <QList>
#include <QLabel>
#include <QPalette>
#include <QString>

void setLayout2p1(QGridLayout * , QWidget * , QList<QLabel*> *);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

// 初始化变量
    QPalette myPalette;
    myPalette.setColor(QPalette::Window, QColor(0, 255, 0));
    QList<QLabel *> * labelList = new QList<QLabel*>;
    for(int i = 0; i < 3; ++i){
        QLabel * temp1 = new QLabel("Label Num: " + QString::number(i + 1, 10));
        temp1 -> setAutoFillBackground(true);
        temp1->setPalette(myPalette);
        labelList->push_back(temp1);
    }

    QGridLayout * layout2p1 = new QGridLayout();
    QWidget * testWidget = new QWidget();

// 布局处理
    setLayout2p1(layout2p1 , testWidget, labelList);

    int result = a.exec();

// 程序指针delete处理
    // 先删除小的，嵌套在大窗体中的小窗体
    while(!labelList->isEmpty()){
        QLabel * temp = labelList -> last();
        labelList -> removeLast();
        delete temp;
    }
    delete labelList;   // 删除label列表
    // 然后删除窗口布局
    delete layout2p1;
    // 最后删除窗口
    delete testWidget;  // 运行widget的析构时，它会将关联的layout析构，还有放在这个layout内的窗体析构

    return  result;
}

void setLayout2p1(QGridLayout * layout2p1, QWidget * testWidget, QList<QLabel*> *labelList)
{
    // 放置窗口，设置窗口布局
    testWidget -> setLayout(layout2p1);
    layout2p1 -> addWidget(labelList->at(0), 0, 0);
    layout2p1 -> addWidget(labelList->at(1), 1, 0);
    layout2p1 -> addWidget(labelList ->at(2), 0, 1, 2, 1);

    // 列比   第0列与第1列之比为 1:2
    layout2p1 -> setColumnStretch(0, 1);
    layout2p1 -> setColumnStretch(1, 2);

    // 行比   第0行与第1行之比为1:1
    layout2p1 -> setRowStretch(0, 1);
    layout2p1 -> setRowStretch(1, 1);

    testWidget -> show();

    return;
}

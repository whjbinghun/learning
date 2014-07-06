#include "demo.h"
#include "ui_demo.h"
#include <QToolButton>
#include "back_paly.h"
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>
#include <QMovie>


Demo::Demo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Demo)
{
    ui->setupUi(this);

    //setMouseTracking 设置鼠标跟踪
    ui->listView->setMouseTracking(1);




    QToolButton *buttonNew = new QToolButton;//定义buttonNew控件
    buttonNew->setText("显示窗口");
    QToolButton *buttonOpen=new QToolButton; //定义buttonOpen控件
    buttonOpen->setText("回放模块");
    ui->mainToolBar->addWidget(buttonNew);
    ui->mainToolBar->addWidget(buttonOpen);

    //播放
    connect(buttonNew,SIGNAL(clicked()),this,SLOT(slot_play()));
    //回放
    connect(buttonOpen,SIGNAL(clicked()),this,SLOT(slot_back()));

    o_back_play = new Back_Paly(this);

    ui->gridLayout->addWidget(o_back_play);
    o_back_play->hide();

    //list_view();
    ui->centralWidget -> setLayout(ui->gridLayout);
        ui->gridLayout -> addWidget(ui->listView, 0, 0, 2, 1);
        ui->gridLayout -> addWidget(ui->frame, 0, 1);
        ui->gridLayout -> addWidget(ui->tableView_2, 1, 1);
        ui->gridLayout -> addWidget(ui->frame_2, 0, 2);
        ui->gridLayout -> addWidget(ui->frame_3, 1, 2);
        // 列比   第0列与第1列之比为 1:2
        ui->gridLayout -> setColumnStretch(0, 1);
        ui->gridLayout -> setColumnStretch(1, 10);
        ui->gridLayout -> setColumnStretch(2, 10);
        // 行比   第0行与第1行之比为1:1
        ui->gridLayout -> setRowStretch(0, 2);
        ui->gridLayout -> setRowStretch(1, 1);
}

Demo::~Demo()
{
    delete ui;
}

//播放
void Demo::slot_play(){
    //list_view();
    o_back_play->hide();
    // 放置窗口，设置窗口布局
    /*ui->centralWidget -> setLayout(ui->gridLayout);
    ui->gridLayout -> addWidget(ui->listView, 0, 0, 2, 1);
    ui->gridLayout -> addWidget(ui->frame, 0, 1);
    ui->gridLayout -> addWidget(ui->tableView_2, 1, 1);
    ui->gridLayout -> addWidget(ui->frame_2, 0, 2);
    ui->gridLayout -> addWidget(ui->frame_3, 1, 2);
    // 列比   第0列与第1列之比为 1:2
    ui->gridLayout -> setColumnStretch(0, 1);
    ui->gridLayout -> setColumnStretch(1, 10);
    ui->gridLayout -> setColumnStretch(2, 10);
    // 行比   第0行与第1行之比为1:1
    ui->gridLayout -> setRowStretch(0, 2);
    ui->gridLayout -> setRowStretch(1, 1);
*/
    ui->frame->show();
    ui->frame_2->show();
    ui->frame_3->show();
    ui->tableView_2->show();
    ui->listView->show();
    //o_back_play->hide();
}

//回放
void Demo::slot_back(){
    //list_view();
    ui->frame->hide();
    ui->frame_2->hide();
    ui->frame_3->hide();
    ui->tableView_2->hide();
    ui->listView->hide();
    //ui->gridLayout->addWidget(o_back_play);
    o_back_play->show();


}

void Demo::OnDoubleClickTree(const QModelIndex &index){
    int devicerow = index.row();
    qDebug()<<devicerow;
}

void Demo::list_view(){
    standard_item = new QStandardItemModel(this);

     /*****************初始化Item数组************************************/
     QStringList strList;
     strList.append("a");
     strList.append("b");
     strList.append("c");
     strList.append("d");
     strList.append("e");
     strList.append("f");
     strList.append("g");
     strList << "h";
     strList += "i";
     int nCount = strList.size();//获取strList的大小

     /****************初始化model*************************************/
     for(int i = 0; i < nCount; i++)
     {
         QString string = static_cast<QString>(strList.at(i));
         QStandardItem *item = new QStandardItem(string);
         if(i %2 == 1)
         {
             QLinearGradient linearGrad(QPointF(0, 0), QPointF(200, 200));
             linearGrad.setColorAt(0, Qt::darkGreen);
             linearGrad.setColorAt(1, Qt::yellow);
             QBrush brush(linearGrad);
             item->setBackground(brush);
         }
         standard_item->appendRow(item);

     }
     ui->listView->setModel(standard_item);//显示model中的内容
     connect(ui->listView,SIGNAL(doubleClicked(const QModelIndex&)),\
     this,SLOT(on_listView_doubleClicked(const QModelIndex&)));

}

//点击
void Demo::on_listView_clicked(const QModelIndex &index)
{

}

//双击
void Demo::on_listView_doubleClicked(const QModelIndex &index)
{
    int devicerow = index.row();
    qDebug()<<devicerow;


    //QMovie *movie = new QMovie("../111.mp4");
    //ui->label->setMovie(movie);
    // 开始播放
    //movie->start();

    WId h_frame = ui->frame->winId();
    ui->frame->setObjectName("红外视频播放");
    ui->frame->resize(320,240);

    // 在frame中添加动画
    //ui->frame-> setMovie(movie);


    ui->frame->show();

}

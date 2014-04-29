#include "gallery.h"
#include "ui_gallery.h"
#include <QProgressBar>
#include <QSlider>
#include <qlistview.h>
#include <QModelIndex>
#include <QSpinBox>
#include <QKeySequenceEdit>
#include <QDebug>
#include <QTableWidget>
#include <QScrollBar>
//#include <QColumnView>

Gallery::Gallery(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Gallery)
{
    ui->setupUi(this);


    //QSpinBox的使用
    ui->spinBox->setMinimum(-20);
    ui->spinBox->setMaximum(100);
    ui->spinBox_2->setMinimum(0);
    ui->spinBox_2->setMaximum(99);

    //QKeySequenceEdit的使用
    //connect(ui->keySequenceEdit,SIGNAL(windowIconTextChanged(QString)), this, SLOT(setWindowTitle(QString)));
    connect(ui->keySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), ui->keySequenceEdit, SLOT(setKeySequence(QKeySequence)));

    ui->progressBar->setValue(0);
    ui->progressBar->setRange(0,100);

    //让QProgressBar显示为等待状态
    ui->progressBar_2->setRange(0,0);

    //QProgressBar和QSlider的使用
    connect(ui->horizontalSlider,SIGNAL(sliderMoved(int)),this,SLOT(move_bar(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(change_bar(int)));


    QStringList headers;//存放列标题QSting组
    headers<<"#"<<"Title"<<"Credit"<<"Image source"<<"";
    ui->tableWidget->setColumnCount(5);//设置列数
    ui->tableWidget->setRowCount(0);//设置行数
    ui->tableWidget->setHorizontalHeaderLabels(headers);//设置列标题
    ui->tableWidget->resizeColumnsToContents();//这个非必需，作用是将所有列根据列的数据宽度，标题宽度重新设置大小，可以防止显示不完整
    //设置标题的默认间隔大小
    //ui->tableWidget->horizontalHeader()->setDefaultSectionSize(146);
    //设置充满表宽度
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"\
     "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"\
     "QScrollBar::handle:hover{background:gray;}"\
     "QScrollBar::sub-line{background:transparent;}"\
     "QScrollBar::add-line{background:transparent;}");
        ui->tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"\
     "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"\
     "QScrollBar::handle:hover{background:gray;}"\
     "QScrollBar::sub-line{background:transparent;}"\
     "QScrollBar::add-line{background:transparent;}");

    int n_num = 0;
    QString s_num;
    QString s_title="A title ";
    QString s_credit="N/A";
    QString s_image = "http://someurl.com";
    QString s_null = "";
    //int row_count = ui->tableWidget->rowCount(); //获取表单行数
    //ui->tableWidget->insertRow(row_count); //插入新行
     ui->tableWidget->setShowGrid(false); //设置不显示格子线
     //去掉默认的行号
     QHeaderView* headerView = ui->tableWidget->verticalHeader();
     headerView->setHidden(true);

     ui->tableWidget->setColumnWidth(1,100);
     ui->tableWidget->setColumnWidth(2,100);
     ui->tableWidget->setColumnWidth(3,200);

     //设置对应的图标、文件名称、最后更新时间、对应的类型、文件大小
    //item->setIcon(icon); //icon为调用系统的图标，以后缀来区分
    for( int i = 0; i< 30; i++) {
        QTableWidgetItem *item = new QTableWidgetItem();
        QTableWidgetItem *item1 = new QTableWidgetItem();
        QTableWidgetItem *item2 = new QTableWidgetItem();
        QTableWidgetItem *item3 = new QTableWidgetItem();
        QTableWidgetItem *item4 = new QTableWidgetItem();
        int n_row_count = ui->tableWidget->rowCount(); //获取表单行数
        ui->tableWidget->insertRow(n_row_count); //插入新行
        n_num = n_num +1;
        s_num = QString("%1").arg(n_num);
        item->setText(s_num);
        item1->setText(QString("%1 %2").arg(s_title).arg(n_num));
        item2->setText(s_credit); //type为调用系统的类型，以后缀来区分
        item3->setText(s_image);
        item4->setText(s_null);
        ui->tableWidget->setItem(n_row_count, 0, item);
        ui->tableWidget->setItem(n_row_count, 1, item1);
        ui->tableWidget->setItem(n_row_count, 2, item2);
        ui->tableWidget->setItem(n_row_count, 3, item3);
        ui->tableWidget->setItem(n_row_count, 4, item4);
        if( i % 2 == 1) {
            QLinearGradient linearGrad(QPointF(0, 0), QPointF(731, 421));
            linearGrad.setColorAt(0, Qt::darkGreen);
            linearGrad.setColorAt(1, Qt::yellow);
            QBrush brush(linearGrad);
            item->setBackground(brush);
            item1->setBackground(brush);
            item2->setBackground(brush);
            item3->setBackground(brush);
            item4->setBackground(brush);
        }
    }

    connect(ui->tableWidget,SIGNAL(clicked(QModelIndex)),this,SLOT(itemClicked(QModelIndex)));

}

Gallery::~Gallery()
{
    delete ui;
}

void Gallery::move_bar(int value){
    ui->horizontalSlider->setValue(value);
    ui->progressBar->setValue(value);
}

void Gallery::change_bar(int value){
    ui->horizontalSlider->setValue(value);
    ui->progressBar->setValue(value);
}

void Gallery::itemClicked(QModelIndex index)
{
    qDebug() << index.data().toString();
}

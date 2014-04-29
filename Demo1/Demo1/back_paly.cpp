#include "back_paly.h"
#include "ui_back_paly.h"

Back_Paly::Back_Paly(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Back_Paly)
{
    ui->setupUi(this);
    //ui->dateTimeEdit->time();
    // 放置窗口，设置窗口布局
    //parent->setLayout(ui->gridLayout);
    /*
    ui->gridLayout -> addWidget(ui->listView, 0, 0);
    ui->gridLayout -> addWidget(ui->frame, 0, 1);
    //位置1行1列；占有：1行，2列
    ui->gridLayout -> addWidget(ui->frame_3, 1, 1,1,2);
    ui->gridLayout -> addWidget(ui->frame_2, 0, 2);
    // 列比   第0列与第1列之比为 1:2
    ui->gridLayout -> setColumnStretch(0, 1);
    ui->gridLayout -> setColumnStretch(1, 10);
    ui->gridLayout -> setColumnStretch(2, 10);
    // 行比   第0行与第1行之比为1:1
    ui->gridLayout -> setRowStretch(0, 2);
    ui->gridLayout -> setRowStretch(1, 1);*/

}

Back_Paly::~Back_Paly()
{
    delete ui;
}

void Back_Paly::on_pushButton_clicked()
{

}

//日期改变
void Back_Paly::on_dateTimeEdit_dateChanged(const QDate &date)
{

}
//
void Back_Paly::on_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{

}

void Back_Paly::on_dateTimeEdit_timeChanged(const QTime &time)
{

}

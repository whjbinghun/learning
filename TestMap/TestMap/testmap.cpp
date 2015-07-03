#include "testmap.h"
#include "ui_testmap.h"
#include <QDebug>

TestMap::TestMap(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestMap)
{
    ui->setupUi(this);

    QMap<int, QString> map_test;
    QString str1 = "askfjsd";
    QString str2 = "bskfjsdkf";
    QString str3 = "csfkjsdkfdsfsdf";
    map_test.insert( 1, str1 );
    map_test.insert( 3, str3 );
    map_test.insert( 2, str2 );
    qDebug()<<"TestMap"<<map_test;
}

TestMap::~TestMap()
{
    delete ui;
}

#include "testpixmap.h"
#include "ui_testpixmap.h"
#include <QPixmap>

#define TITLE_H 50

TestPixmap::TestPixmap(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestPixmap)
{
    ui->setupUi(this);
    //图像标签--logo
    mp_label_icon = new QLabel( this );
    QPixmap objPixmap( "./image/logo.png");
    mp_label_icon->setPixmap( objPixmap.scaled( TITLE_H, TITLE_H ) );


    ui->label->setPixmap(QPixmap("logo.png"));
    //文本标签--标题
    m_pLabelTitle = new QLabel(this);
    m_pLabelTitle->setText(QString("360 Safe Guard V8.5"));

}

TestPixmap::~TestPixmap()
{
    delete ui;
}

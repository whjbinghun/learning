#include "viframergb.h"
#include "ui_viframergb.h"
#include <QFile>
#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QThread>



ViFrameRGB::ViFrameRGB(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViFrameRGB)
{
    ui->setupUi(this);

    mq_tool_button = new QToolButton;
    mq_tool_button->setText("显示窗口");
    ui->mainToolBar->addWidget(mq_tool_button);
    mq_tool_button->setStyleSheet("color:red");

    m_image = QImage( 1920, 1080, QImage::Format_RGBA8888);

    //将定时器超时信号与槽(功能函数)联系起来
    my_thread.set_image( &m_image );
    my_thread.start();
    connect( &my_thread,SIGNAL(signal_play_wnd()), this, SLOT(slot_play_wnd()) );
}

ViFrameRGB::~ViFrameRGB()
{
    my_thread.exit_thread();
    my_thread.quit();
    my_thread.wait();
    delete ui;
}

void ViFrameRGB::paintEvent(QPaintEvent * event) {
    QPainter draw;
    draw.begin( this );

    //红外显示
    if( m_image.isNull() ) {
        draw.fillRect( 1, 1, 1920, 1080, QBrush( QColor( 200, 200, 200 )));
    } else {
        //使用ui->centralWidget->rect()可以使得窗口是可变化的
        draw.drawImage( ui->centralWidget->rect(), m_image, QRect(0, 0, 1920, 1080) );
    }
    draw.end();
}

void ViFrameRGB::slot_play_wnd()
{
    update();
}

void ViFrameRGB::timer_requset()
{
    //读取文件
    QString s_file_name = "../monitor.rgb";
    QFile file(s_file_name);

    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"Can't open the file!"<<endl;
    }

    quint64 n_len = 1920*1080*4;
    char buf[n_len];

    int i=0;


    while(!file.atEnd()) {
    //if(!file.atEnd()) {
        //memset(buf, 0, n_len);
        file.read(buf, n_len);
        //memcpy(m_image, file.read(n_len), n_len);
        usleep(40*1000);
        memcpy( m_image.bits(), buf, n_len);
        i=i+1;
        qDebug()<<i;
        update();
     }
}




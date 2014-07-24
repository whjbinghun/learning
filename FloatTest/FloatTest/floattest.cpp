#include "floattest.h"
#include "ui_floattest.h"
#include <QDebug>

FloatTest::FloatTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FloatTest)
{
    ui->setupUi(this);
    init();
}

FloatTest::~FloatTest()
{
    delete ui;
}

void FloatTest::init()
{
    MONITOR_TEMPERATURE_PKT_S max_temp_pkt;
    char a[5] = {0x12,0x34,0x56,0x78,0x9a};
    memcpy( &max_temp_pkt, a, 5 );
    int pt_x=0;
    int pt_y=0;
    int n_t1=0;
    int len = 4;
    /*int max = (max_temp_pkt.c[0]<<2)+(max_temp_pkt.c[1]>>6);
    int min = ((max_temp_pkt.c[1]&0x3F)<<3)+(max_temp_pkt.c[2]>>5);
    int avg = ((max_temp_pkt.c[2]&0x1F)<<8)+max_temp_pkt.c[3];*/
    temp_pkt_ana( (max_temp_pkt.c), len, pt_x, pt_y, n_t1 );



    int adfd = max_temp_pkt.t2;
    qDebug()<<pt_x<<pt_y<<n_t1<<adfd;
    /*max_temp_pkt.x = 100;
    max_temp_pkt.y = 200;
    max_temp_pkt.t1 = 10;
    max_temp_pkt.t2 = 20;*/
    //float f_test = (float)max_temp_pkt.t2;
    float f_temp=0;
    judge_int_len( max_temp_pkt.t2, f_temp);
    qDebug()<<sizeof( MONITOR_TEMPERATURE_PKT_S)<<pt_x<<pt_y<<n_t1<<adfd<<f_temp<<max_temp_pkt.c[0];
}

void FloatTest::judge_int_len( int n_temp, float &f_temp )
{
    int j = 0;
    while( n_temp>=1 )
    {
        j = n_temp%10;
        n_temp=n_temp/10;
        f_temp = (f_temp+j)*0.1;
    }
}

void FloatTest::temp_pkt_ana( quint8 *temp_pkt, int len, int &pt_x, int &pt_y, int &n_t1 )
{
    pt_x=0;
    pt_y = 0;
    n_t1 = 0;
    pt_x = (temp_pkt[0]<<2)+(temp_pkt[1]>>6);
    pt_y = ((temp_pkt[1]&0x3F)<<3)+(temp_pkt[2]>>5);
    n_t1 = ((temp_pkt[2]&0x1F)<<8)+temp_pkt[3];

}

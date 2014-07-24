#ifndef FLOATTEST_H
#define FLOATTEST_H

#include <QMainWindow>

#pragma pack(push)
#pragma pack(1)
typedef struct tagMONITOR_TEMPERATURE_PKT_S{
    //quint32 x:10;//x轴坐标（范围为0~1023，全为1表示无坐标的温度）
    //quint32 y:9;// y轴坐标（范围为0~511，全为1表示无坐标的温度）
    //quint32 t1;//温度的整数位 （0~8191）
    quint8 c[4];
    quint8 t2;//温度的小数位  （0~255）
}MONITOR_TEMPERATURE_PKT_S;
#pragma pack(pop)

namespace Ui {
class FloatTest;
}

class FloatTest : public QMainWindow
{
    Q_OBJECT

public:
    explicit FloatTest(QWidget *parent = 0);
    ~FloatTest();
public:
    void init();
    void judge_int_len( int n_temp, float &f_temp );
    void temp_pkt_ana(quint8 *temp_pkt, int len, int &pt_x, int &pt_y, int &n_t1 );
private:
    Ui::FloatTest *ui;
};

#endif // FLOATTEST_H



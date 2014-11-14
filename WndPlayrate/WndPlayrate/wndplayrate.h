#ifndef WNDPLAYRATE_H
#define WNDPLAYRATE_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class WndPlayrate;
}

typedef struct _tagRateInfo
{
    float f_position;//文本位置
    QString str_text_name;
    float f_value;
}RateInfo;

class WndPlayrate : public QWidget
{
    Q_OBJECT

public:
    explicit WndPlayrate(QWidget *parent = 0);
    ~WndPlayrate();
public:
    void paintEvent( QPaintEvent *p_event );
    void resizeEvent( QResizeEvent *p_event );
    void mousePressEvent( QMouseEvent *p_event );
    void init_pt_pos();
    void init_graphical();
    void insert_mark( float f_pos, float f_value, QString str_text );
    void cal_mark_pos();
signals:
    void sig_rate_num( float );
private:
    Ui::WndPlayrate *ui;
    QVector<QPoint> m_vector_pt;
    QVector<RateInfo> m_vector_rate;
    int mn_slider_position;
};

#endif // WNDPLAYRATE_H

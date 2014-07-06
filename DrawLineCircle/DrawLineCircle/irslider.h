#ifndef IRSLIDER_H
#define IRSLIDER_H

#include <QSlider>
#include <QMouseEvent>

class IrSlider : public QSlider
{
    Q_OBJECT
public:
    explicit IrSlider(QWidget *parent = 0);
    ~IrSlider();
public:
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );

    void set_value_pos( int n_pos );

    bool get_value_status();
    int get_value_pos();
signals:
    void signal_value_change( int n_pos );

public slots:
    void slot_slider_value();
private:
    int mn_pos;
    bool mb_value_change_status;
};

#endif // IRSLIDER_H

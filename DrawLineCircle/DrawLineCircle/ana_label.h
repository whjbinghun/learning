#ifndef ANA_LABEL_H
#define ANA_LABEL_H

#include <QLabel>
#include <QMouseEvent>

class Ana_Label : public QLabel
{
    Q_OBJECT
public:
    explicit Ana_Label(QWidget *parent = 0);
    ~Ana_Label();
public:
    enum enum_press_status{
        none_press_status = 0,
        point_status = 1,
        line_status = 2,
        rect_status = 3,
        circle_status = 4
    } me_press_status;
    void init_ir_widget();
    void paintEvent( QPaintEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void press_status_shape( QPoint pt );

    enum_press_status get_mouse_press_status();
signals:

public slots:

private:

    QList<QString> m_list;
};

#endif // ANA_LABEL_H

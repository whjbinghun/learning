#ifndef DELETELABEL_H
#define DELETELABEL_H

#include <QLabel>
#include <QMouseEvent>

class Ana_Label;

class DeleteLabel : public QLabel
{
    Q_OBJECT
public:
    explicit DeleteLabel(QWidget *parent = 0);
    ~DeleteLabel();
public:
    void resizeEvent( QResizeEvent *event );
    void paintEvent( QPaintEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    bool get_delete_status();
    void set_delete_status( bool b_delete_status );
    void set_ana_label( Ana_Label *p_ana_label );
private:
    bool mb_delete_status;
    Ana_Label *mp_ana_label;
};

#endif // DELETELABEL_H

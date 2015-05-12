#ifndef DELETELABEL_H
#define DELETELABEL_H

#include <QLabel>
#include <QMouseEvent>


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

signals:
    void sig_delete_shape();

private:
    bool mb_delete_status;
};

#endif // DELETELABEL_H

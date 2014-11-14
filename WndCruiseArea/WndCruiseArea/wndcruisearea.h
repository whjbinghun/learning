#ifndef WNDCRUISEAREA_H
#define WNDCRUISEAREA_H

#include <QWidget>

namespace Ui {
class WndCruiseArea;
}

class WndCruiseArea : public QWidget
{
    Q_OBJECT

public:
    explicit WndCruiseArea(QWidget *parent = 0);
    ~WndCruiseArea();
public:
    void resizeEvent( QResizeEvent *event );

    void move_widget();
    void move_group_name( QRect rct );
    void move_combox( QRect rct );
    void move_btn( QRect rct );
    void move_image( QRect rct );
private:
    Ui::WndCruiseArea *ui;
};

#endif // WNDCRUISEAREA_H

#ifndef WNDVIFRAME_H
#define WNDVIFRAME_H

#include <QFrame>

class WndViFrame : public QFrame
{
    Q_OBJECT
public:
    explicit WndViFrame(QWidget *parent = 0);
    ~WndViFrame();
    void keyPressEvent( QKeyEvent *event );
signals:

public slots:
private:

};

#endif // WNDVIFRAME_H

#ifndef WNDIRFRAME_H
#define WNDIRFRAME_H

#include <QFrame>

class WndIrFrame : public QFrame
{
    Q_OBJECT
public:
    explicit WndIrFrame(QWidget *parent = 0);
    ~WndIrFrame();
    void keyPressEvent( QKeyEvent *event );
signals:

public slots:
private:

};

#endif // WNDIRFRAME_H

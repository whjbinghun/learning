#ifndef IRANAVIFOCUSE_H
#define IRANAVIFOCUSE_H

#include <QWidget>

namespace Ui {
class IrAnaViFocuse;
}

class IrAnaViFocuse : public QWidget
{
    Q_OBJECT

public:
    explicit IrAnaViFocuse(QWidget *parent = 0);
    ~IrAnaViFocuse();
public:
    void resizeEvent( QResizeEvent *event );
    void move_widget();
private:
    Ui::IrAnaViFocuse *ui;
};

#endif // IRANAVIFOCUSE_H

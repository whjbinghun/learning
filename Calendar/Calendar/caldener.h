#ifndef CALDENER_H
#define CALDENER_H

#include <QMainWindow>

namespace Ui {
class Caldener;
}

class Caldener : public QMainWindow
{
    Q_OBJECT

public:
    explicit Caldener(QWidget *parent = 0);
    ~Caldener();

private:
    Ui::Caldener *ui;
};

#endif // CALDENER_H

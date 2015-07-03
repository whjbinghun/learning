#ifndef CALDENER_H
#define CALDENER_H

#include <QMainWindow>
#include <QDate>

namespace Ui {
class Caldener;
}

class Caldener : public QMainWindow
{
    Q_OBJECT

public:
    explicit Caldener(QWidget *parent = 0);
    ~Caldener();

public slots:
    void slot_clicked( QDate date );
private:
    Ui::Caldener *ui;
};

#endif // CALDENER_H

#ifndef BACK_PALY_H
#define BACK_PALY_H

#include <QWidget>
#include "ui_back_paly.h"

namespace Ui {
class Back_Paly;
}

class Back_Paly : public QWidget
{
    Q_OBJECT

public:
    explicit Back_Paly(QWidget *parent = 0);
    ~Back_Paly();

private slots:
    void on_pushButton_clicked();

    void on_dateTimeEdit_dateChanged(const QDate &date);

    void on_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime);

    void on_dateTimeEdit_timeChanged(const QTime &time);

private:
    Ui::Back_Paly *ui;
};

#endif // BACK_PALY_H

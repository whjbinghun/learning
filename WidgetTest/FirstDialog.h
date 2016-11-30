#ifndef FIRSTDIALOG_H
#define FIRSTDIALOG_H

#include <QDialog>
#include "SecondWidget.h"

namespace Ui {
class FirstDialog;
}

class FirstDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FirstDialog(QWidget *parent = 0);
    ~FirstDialog();
private slots:
    void slot_showSecondDialog();
private:
    Ui::FirstDialog *ui;

    SecondWidget *m_p;
};

#endif // FIRSTDIALOG_H

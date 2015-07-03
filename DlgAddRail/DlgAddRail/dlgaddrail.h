#ifndef DLGADDRAIL_H
#define DLGADDRAIL_H

#include <QDialog>

namespace Ui {
class DlgAddRail;
}

class DlgAddRail : public QDialog
{
    Q_OBJECT

public:
    explicit DlgAddRail(QWidget *parent = 0);
    ~DlgAddRail();
public:
    void init_ctrl();
private:
    Ui::DlgAddRail *ui;
};

#endif // DLGADDRAIL_H

#ifndef DLGSETANAANDALARM_H
#define DLGSETANAANDALARM_H

#include <QDialog>

namespace Ui {
class DlgSetAnaAndAlarm;
}

class DlgSetAnaAndAlarm : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSetAnaAndAlarm(QWidget *parent = 0);
    ~DlgSetAnaAndAlarm();

private:
    Ui::DlgSetAnaAndAlarm *ui;
};

#endif // DLGSETANAANDALARM_H

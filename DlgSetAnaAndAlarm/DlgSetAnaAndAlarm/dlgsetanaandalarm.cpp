#include "dlgsetanaandalarm.h"
#include "ui_dlgsetanaandalarm.h"

DlgSetAnaAndAlarm::DlgSetAnaAndAlarm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSetAnaAndAlarm)
{
    ui->setupUi(this);
}

DlgSetAnaAndAlarm::~DlgSetAnaAndAlarm()
{
    delete ui;
}

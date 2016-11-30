#include "FirstDialog.h"
#include "ui_FirstDialog.h"
#include "SecondDialog.h"
#include "SecondWidget.h"

FirstDialog::FirstDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FirstDialog)
    , m_p( NULL )
{
    ui->setupUi(this);
    setWindowFlags( Qt::FramelessWindowHint);

    m_p = new SecondWidget;
    connect( ui->pushButton, SIGNAL(clicked(bool)), this, SLOT( slot_showSecondDialog() ) );
}

FirstDialog::~FirstDialog()
{
    delete ui;
}

void FirstDialog::slot_showSecondDialog()
{
//    SecondDialog *p = new SecondDialog;

    m_p->activateWindow();
    m_p->show();
}

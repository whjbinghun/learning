#include "caldener.h"
#include "ui_caldener.h"

Caldener::Caldener(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Caldener)
{
    ui->setupUi(this);

    ui->id_calendarWidget->setSelectionMode( QCalendarWidget::NoSelection );
    connect( ui->id_calendarWidget, SIGNAL(activated(QDate)), SLOT( slot_clicked( QDate ) ) );
}

Caldener::~Caldener()
{
    delete ui;
}

void Caldener::slot_clicked( QDate date )
{
    ui->id_calendarWidget->setSelectedDate( date );
}

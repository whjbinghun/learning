#include "dlgaddrail.h"
#include "ui_dlgaddrail.h"

DlgAddRail::DlgAddRail(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAddRail)
{
    ui->setupUi(this);
    setWindowFlags( Qt::Dialog | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint );
    setWindowTitle( "新增导轨" );
    init_ctrl();
}

DlgAddRail::~DlgAddRail()
{
    delete ui;
}

void DlgAddRail::init_ctrl()
{
    QRegExpValidator *p_ip_validator = new QRegExpValidator( QRegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)"), this );
    QRegExpValidator *p_port_validator = new QRegExpValidator( QRegExp("[0-9]{10}"), this );
    ui->id_rail_edit_ip->setValidator( p_ip_validator );
    ui->id_rail_edit_port->setValidator( p_port_validator );

    ui->id_rail_edit_port->setText( "2000" );
    ui->id_rail_edit_port->setEnabled( false );
}


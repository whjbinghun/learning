#include "addpresetdialog.h"
#include "ui_addpresetdialog.h"

AddPresetDialog::AddPresetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPresetDialog)
{
    ui->setupUi(this);
    init_connect();
}

AddPresetDialog::~AddPresetDialog()
{
    delete ui;
}

void AddPresetDialog::init_connect()
{
    connect( ui->id_btn_sure, SIGNAL( clicked() ), this, SLOT( slot_get_line() ) );
    connect( ui->id_btn_cancel, SIGNAL( clicked() ), this, SLOT( slot_close_dialog() ) );
}

void AddPresetDialog::slot_get_line()
{
    ui->id_edit_preset->text();
}

void AddPresetDialog::slot_close_dialog()
{
    close();
}

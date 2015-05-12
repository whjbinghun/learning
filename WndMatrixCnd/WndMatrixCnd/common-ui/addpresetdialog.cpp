#include "addpresetdialog.h"
#include "ui_addpresetdialog.h"
#include <QRegExp>
#include <QValidator>
#include <QMessageBox>

AddPresetDialog::AddPresetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPresetDialog)
    ,mb_sure_press( false )
{
    ui->setupUi(this);
    init_connect();


    QRegExp regx("^[^Aa-]{255}"); // {50}
    QValidator *validator = new QRegExpValidator(regx, ui->id_edit_preset);
    ui->id_edit_preset->setValidator(validator);
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

bool AddPresetDialog::get_sure_press()
{
    return mb_sure_press;
}

QString AddPresetDialog::get_preset_text()
{
    return ui->id_edit_preset->text();
}

void AddPresetDialog::set_edit_text( QString str )
{
    ui->id_edit_preset->setText( str );
}

void AddPresetDialog::set_static_text(QString str)
{
    ui->id_lab_preset->setText( str );
}

void AddPresetDialog::slot_get_line()
{
    QString str = ui->id_edit_preset->text();
    if ( str.isEmpty() ) {
        QMessageBox::information( this, "提示", "输入不能为空！" );
        return;
    }
    mb_sure_press = true;
    accept();
    close();
}

void AddPresetDialog::slot_close_dialog()
{
    mb_sure_press = false;
    reject();
    close();
}

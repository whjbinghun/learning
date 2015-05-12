#ifndef ADDPRESETDIALOG_H
#define ADDPRESETDIALOG_H

#include <QDialog>

namespace Ui {
class AddPresetDialog;
}

class AddPresetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPresetDialog(QWidget *parent = 0);
    ~AddPresetDialog();
public:
    void init_connect();
    bool get_sure_press();
    QString get_preset_text();
    void set_edit_text( QString str );
    void set_static_text( QString str );

public slots:
    void slot_close_dialog();
    void slot_get_line();
private:
    Ui::AddPresetDialog *ui;
    bool mb_sure_press;
};

#endif // ADDPRESETDIALOG_H

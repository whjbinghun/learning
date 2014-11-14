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
public slots:
    void slot_close_dialog();
    void slot_get_line();
private:
    Ui::AddPresetDialog *ui;
};

#endif // ADDPRESETDIALOG_H

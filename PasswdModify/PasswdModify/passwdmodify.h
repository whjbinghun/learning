#ifndef PASSWDMODIFY_H
#define PASSWDMODIFY_H

#include <QDialog>

namespace Ui {
class PasswdModify;
}

class PasswdModify : public QDialog
{
    Q_OBJECT

public:
    explicit PasswdModify(QWidget *parent = 0);
    ~PasswdModify();

private:
    Ui::PasswdModify *ui;
};

#endif // PASSWDMODIFY_H

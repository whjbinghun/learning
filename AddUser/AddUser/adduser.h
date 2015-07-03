#ifndef ADDUSER_H
#define ADDUSER_H

#include <QDialog>

namespace Ui {
class AddUser;
}

class AddUser : public QDialog
{
    Q_OBJECT

public:
    explicit AddUser(QWidget *parent = 0);
    ~AddUser();
public:
    void init_user();
    void init_connect();
    void passwd_diff();
public slots:
    void slot_click_sure();
    void slot_click_cancel();
private:
    Ui::AddUser *ui;
};

#endif // ADDUSER_H

#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
//#include <QSqlTableModel>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "cfgfileinfo.h"
#include "titlebar.h"


class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = 0);
public:
    QString get_IP();
    QString get_port();
    QString get_name();
    QString get_pwd();
    void login_set_text();
    void init_login();
    void init_title_bar();
    void resizeEvent( QResizeEvent *event );
    void paintEvent( QPaintEvent *event );
public slots:
    void login_clicked();
private:
    QLabel *mp_lab_IP;
    QLabel *mp_lab_port;
    QLabel *mp_lab_name;
    QLabel *mp_lab_pwd;
    QLineEdit *mp_line_IP;
    QLineEdit *mp_line_port;
    QLineEdit *mp_line_name;
    QLineEdit *mp_line_pwd;
    QPushButton *btn_Login;
    QPushButton *btn_Cancle;
    QString ms_IP;
    QString ms_port;
    QString ms_name;
    QString ms_pwd;
    CfgFileInfo *mp_cfg_file_info;
    QString ms_dir_path;
    QString ms_file_name;
private:
    TitleBar *mp_title_bar;
};

#endif // LOGINDIALOG_H

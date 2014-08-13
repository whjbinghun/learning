#ifndef DIALOGTITLEBAR_H
#define DIALOGTITLEBAR_H

#include <QDialog>
#include "titlebar.h"
#include <QMouseEvent>
#include "logindialog.h"

namespace Ui {
class DialogTitleBar;
}

class DialogTitleBar : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTitleBar(QWidget *parent = 0);
    ~DialogTitleBar();
public:
    void init_title_bar();
    void paintEvent( QPaintEvent *event );
    void init_login_dialog();
private:
    Ui::DialogTitleBar *ui;
    TitleBar *mp_title_bar;
    LoginDialog *mp_login_dialog;
};

#endif // DIALOGTITLEBAR_H

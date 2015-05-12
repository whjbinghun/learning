#ifndef NOTIFYDIALOG_H
#define NOTIFYDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class NotifyDialog;
}

class NotifyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NotifyDialog(QWidget *parent = 0);
    ~NotifyDialog();
    void paintEvent( QPaintEvent *event );
    void start_timer();
    void stop_timer();
    void exit_notify_dlg();  //断开连上之后退出该提示框
signals:
    void sig_application_exit();

private slots:
    void on_pushButton_clicked();
    void slot_connect_timer();

private:
    Ui::NotifyDialog *ui;
    QTimer *mp_timer;
    QString m_notify;
    int mn_count;
};

#endif // NOTIFYDIALOG_H

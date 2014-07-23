#ifndef DLGCONFIGPRS_H
#define DLGCONFIGPRS_H

#include <QDialog>
#include <QTimer>
#include <QMutex>

namespace Ui {
    class DlgConfigProgress;
}
class NetControl;

class DlgConfigPrs : public QDialog
{
    Q_OBJECT
public:
    explicit DlgConfigPrs(QWidget *parent = 0);
    ~DlgConfigPrs();

    void set_net_ctrl( NetControl *p_ctrl, int n_cmd_num );
    void close_timer_cmd();
    void close_timer_timeout();

private:
    NetControl *mp_net_ctrl;
    QTimer *mp_timer;
    QTimer *mp_timer_timeout;

    enum {
        cmd_timeout,
        cmd_confirm_group,
        cmd_send_xml,
        cmd_confirm_xml
    };
    int mn_status;
    int mn_cmd_num;
    int mn_cmd_update_xml;
    bool mb_has_fail_cmd;

    QMutex m_mutex_status;

public:
    Ui::DlgConfigProgress *ui;

signals:

public slots:
    void timer_update_progress();
    void timer_cmd_progress_timeout();
};

#endif // DLGCONFIGPRS_H

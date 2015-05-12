#ifndef DLGALARMCONFIG_H
#define DLGALARMCONFIG_H

#include <QDialog>
#include  "alarmcfg.h"

namespace Ui {
class DlgAlarmConfig;
}

class DlgAlarmConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DlgAlarmConfig(QString s_sn,int n_type,QWidget *parent = 0);
    ~DlgAlarmConfig();

    void set_ana_info( QStringList& s_list) { ms_list = s_list;}
    void update_data(QString& s_sn);
    void paintEvent( QPaintEvent *event );
    void hide_ok_cancel();
public slots:
    void slot_on_add();
    void slot_on_del();
    void slot_on_clear();
    void slot_on_capture();
    void slot_on_notcapture();
    void slot_on_dir();
    void slot_on_beep();
    void slot_on_msg();

protected:
    void save_group();
    void insert_group(AlarmGroup *po_group);

private:
    QStringList         ms_list;   // ana info;
    QString             ms_sn;     // device sn
    int                 mn_type;   //0 realtime ir ; 1 playback
    Ui::DlgAlarmConfig *ui;
    QPixmap m_pixmap_background;
};

#endif // DLGALARMCONFIG_H

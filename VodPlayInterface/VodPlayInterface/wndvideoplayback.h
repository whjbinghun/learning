#ifndef WNDVIDEOPLAYBACK_H
#define WNDVIDEOPLAYBACK_H

#include <QWidget>
#include <QPushButton>
#include <qslider.h>
#include <QComboBox>
#include <QTimer>
#include <QTimeEdit>
#include <QDateEdit>
#include "vodplayinterface.h"

class WndIRDisplay;
class WndVIDisplay;
class NetControl;

class WndVideoPlayback : public QWidget
{
    Q_OBJECT
public:
    explicit WndVideoPlayback(QWidget *parent = 0);
    ~WndVideoPlayback();

    void init_ctrl( Ui::VodPlayInterface *ui, NetControl *p_net_ctrl );
    void update_ctrl_info();
    void set_enable_playback_ctrl( bool b_enable );

private:

signals:

public slots:
    void slot_btn_start_or_pause();
    void slot_btn_strop();
    void slot_slider_play_change( int n_value );
    void slot_combo_change_group( int n_index );
    void slot_timer_connect_req();
    void slot_timer_playback_cmd_req();
    void slot_timer_update_slider();

private:
    WndIRDisplay *mp_wnd_ir_display;
    WndVIDisplay *mp_wnd_vi_display;
    QComboBox *mp_combo_group;
    QPushButton *mp_btn_start_or_pause;
    QPushButton *mp_btn_stop;
    QPushButton *mp_btn_pre_frame;
    QSlider *mp_slider_play;
    QTimeEdit *mp_end_time;
    QTimeEdit *mp_start_time;
    QDateEdit *mp_end_date;
    QDateEdit *mp_start_date;

    NetControl *mp_net_ctrl;
    Ui::VodPlayInterface *mp_main_wnd;

private:
    QTimer *mp_timer_playback_cmd;
    QTimer *mp_timer_connect_req;
    QTimer *mp_timer_update_slider;
    bool mb_playback_connect_req;
    int mn_timer_connect_count;
    int mn_replay_cmd;
    bool mb_first_connect;
};

#endif // WNDVIDEOPLAYBACK_H

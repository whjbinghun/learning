#ifndef ALARMINGPICTUREPREVIEW_H
#define ALARMINGPICTUREPREVIEW_H

#include <QWidget>
#include <QTimer>
//#include "titlebar.h"
//#include "datamode.h"

namespace Ui {
class AlarmingPicturePreview;
}

class AlarmingPicturePreview : public QWidget
{
    Q_OBJECT

public:
    explicit AlarmingPicturePreview(QWidget *parent = 0);
    ~AlarmingPicturePreview();
public:
    void paintEvent(QPaintEvent *p_event);
    void resizeEvent( QResizeEvent *p_event );

    void move_widget();
    void move_list( QRect rct );
    void move_ir_vi( QRect rct );
    void move_alarm( QRect rct );
    void move_preset_list( QRect rct );

    void init_group_num();
    void init_title_bar();
    void init_list_font();
signals:
    void close_window();
public slots:
    void slot_exit(){
        exit(0);
    }
private:
    Ui::AlarmingPicturePreview *ui;
    //TitleBar *mp_title_bar;
    QTimer *mp_timer_heartbeat;
};

#endif // ALARMINGPICTUREPREVIEW_H

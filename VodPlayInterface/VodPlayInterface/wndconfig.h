#ifndef WNDCONFIG_H
#define WNDCONFIG_H

#include <QWidget>
#include <QPointF>
#include <QImage>
#include <QLabel>
#include "vodplayinterface.h"
#include "wndcommon.h"

class NetControl;
class PicPreview;
class PicPreviewGroup;
class PicPreviewPan;

class WndConfig : public QWidget
{
    Q_OBJECT
public:
    explicit WndConfig(QWidget *parent = 0);

public:
    void init_ctrl( Ui::VodPlayInterface *p_main_wnd, NetControl *p_net_ctrl );
    void update_group_info();

private:
    void send_group_delete( const QMap< int, DeviceGroup > &map_delete_group );
    void send_group_change( const QMap< int, DeviceGroup > &map_change_group );
    void send_group_add( const  QList< DeviceGroup > &list_add_group );

public:
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void mouseDoubleClickEvent( QMouseEvent *event );
    void resizeEvent( QResizeEvent *event );
    void paintEvent( QPaintEvent *event );

public:
    void mouse_press( const QPointF &pt, const PicPreviewInfo &pic_info, int n_class );
    void mouse_release( const QPointF &pt, int n_class );

signals:

private slots:
    void on_id_config_save_clicked();
    void on_id_config_change_clicked();
    void on_id_config_btn_save_clicked();

private:
    QMap< int, DeviceGroup > m_map_device_group;    //. 已经分组设备
    QList< DeviceInfo > m_list_device_ir_no_group;            //. 未分组设备
    QList< DeviceInfo > m_list_device_vi_no_group;            //. 未分组设备

    PicPreviewInfo m_pic_preview_info_down;                     //.按下图片信息
    NetControl *mp_net_control;

    bool mb_mouse_press;
    QPoint mpt_now_pos;
    QPixmap m_pixmap_image;

    QLabel *mp_label_image;
    PicPreview *mp_wnd_ir_preview;
    PicPreview *mp_wnd_vi_preview;
    PicPreviewPan *mp_wnd_ptz_preview;
    PicPreviewGroup *mp_wnd_group_preview;
    QPushButton *mp_btn_save;
};

#endif // WNDCONFIG_H

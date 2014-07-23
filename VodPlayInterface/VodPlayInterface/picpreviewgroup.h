#ifndef PICPREVIEWGROUP_H
#define PICPREVIEWGROUP_H

#include <QFrame>
#include <QImage>
#include <QScrollBar>
#include <QList>
#include <QPushButton>
#include <QMouseEvent>
#include <QPoint>
#include <QPointF>
#include "wndcommon.h"
#include "netcontrol.h"

class PicPreviewPan;
class PicPreview;

class PicPreviewGroup : public QFrame
{
    Q_OBJECT
public:
    explicit PicPreviewGroup(QWidget *parent = 0);
    ~PicPreviewGroup();

    void paintEvent( QPaintEvent *event );
    void resizeEvent( QResizeEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );

public:
    void init( const QMap< int, DeviceGroup > &map_device );

    void clear_group();
    void delete_group();
    void delete_group( QString str_group_name );
    void delete_group( int n_group_num );
    int add_new_group();

    void set_config_chld_wnd( PicPreview *p_ir, PicPreview* p_vi, PicPreviewPan *p_ptz );
    void resize_scorll();

    bool pt_in_delete_btn_group_num( const QPoint &pt, int &n_group_num );
    bool pt_in_new_btn_group_num( const QPoint &pt );
    bool pt_in_group_num( const QPoint &pt, int &n_group_num );

    bool is_vi_in_group( const int &n_group_num );
    bool is_ir_in_group( const int &n_group_num );
    bool is_ptz_in_group( const int &n_group_num );

    bool add_vi_in_group( const int &n_group_num, const PicPreviewInfo &pic_info );
    bool add_ir_in_group( const int &n_group_num, const PicPreviewInfo &pic_info );
    bool add_ptz_in_group( const int &n_group_num, const PicPreviewInfo &pic_info );

    QList< PicPreviewGroupInfo > &get_groups() { return m_list_pic_preview_group; }

protected:
    bool is_device_in_group( const int &n_class, const int &n_group_num );
    bool add_device_in_group( const int &n_class, const int &n_group_num, const PicPreviewInfo &pic_info );

public slots:
    void set_frame( const QPointF *p_point );

    void slot_add_image();
    void set_v_edit_value(int value);

private:
    int mn_scroll_pos;
    QImage *mp_add_image;
    QImage *mp_delete_image;

    bool mb_move_add_image;
    bool mb_press_add_image;

    int  mn_group_number;

    QList< PicPreviewGroupInfo > m_list_pic_preview_group;

    QScrollBar *mp_v_scrollBar;
    PicPreview *mp_wnd_ir, *mp_wnd_vi;
    PicPreviewPan *mp_wnd_ptz;
};


#endif // PICPREVIEWGROUP_H

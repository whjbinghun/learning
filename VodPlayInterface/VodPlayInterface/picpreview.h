#ifndef PICPREVIEW_H
#define PICPREVIEW_H

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

class WndConfig;

class PicPreview : public QFrame
{
    Q_OBJECT
public:
    explicit PicPreview(QWidget *parent = 0);
    ~PicPreview();

    void paintEvent( QPaintEvent *event );
    void resizeEvent( QResizeEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseReleaseEvent(  QMouseEvent *event  );

public:
    void init( const QList< DeviceInfo > &list_device );
    void set_config_wnd( WndConfig *p_wnd_config, int n_class );
    int get_preview_width();
    void cal_scroll_resize();
    void delete_image( QString str_sn );

private:
    bool pt_in_image_rect( const QPointF *p_point, PicPreviewInfo &pic_info );

signals:

public slots:
    void set_line_edit_value( int value );

    void clear_image();
    void delete_image();
    int add_image( QString str_file_path, QString str_sn );

    void slot_add_image();
    void slot_add_image( const PicPreviewInfo &pic_info );

private:
    QScrollBar *mp_scrollBar;
    int mn_scroll_pos;
    QPushButton *mp_erase;
    QPushButton *mp_add_button;

    QString ms_image_name;

    QMap< QString, PicPreviewInfo > m_map_pic_preview;
    WndConfig *mp_wnd_config;
    int mn_class;       // 0: 红外 1:可见光 其他云台
};

#endif // PICPREVIEW_H

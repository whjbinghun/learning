#ifndef PICPREVIEWPAN_H
#define PICPREVIEWPAN_H

#include <QFrame>
#include <QImage>
#include <QScrollBar>
#include <QMap>
#include <QPushButton>
#include <QMouseEvent>
#include <QPoint>
#include <QPointF>
#include "wndcommon.h"

class WndConfig;

class PicPreviewPan : public QFrame
{
    Q_OBJECT
public:
    explicit PicPreviewPan(QWidget *parent = 0);
    ~PicPreviewPan();

    void paintEvent( QPaintEvent *event );
    void resizeEvent( QResizeEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseReleaseEvent(  QMouseEvent *event  );

public:
    void set_config_wnd( WndConfig *p_wnd_config ) { mp_wnd_config = p_wnd_config; }

    void clear_ptz();
    bool add_ptz( QString str_sn );
    bool delete_ptz( QString str_sn );

    void resize_scorll();

private:
    bool pt_in_image_rect( const QPointF *p_point, PicPreviewInfo &pic_info );

signals:

public slots:
    void set_line_edit_value( int value );

    void slot_add_image();
    void slot_add_ptz( const PicPreviewInfo &pic_info );

private:
    QPushButton *mp_erase;
    QPushButton *mp_add_button;

    QScrollBar *mp_scrollBar;
    int mn_scroll_pos;

    QString ms_image_name;

    QMap< QString, PicPreviewInfo > m_map_pic_pan;
    int mn_class;
    WndConfig *mp_wnd_config;
};


#endif // PICPREVIEWPAN_H

#ifndef PICPRECONFIG_H
#define PICPRECONFIG_H

#include <QWidget>
#include <QPointF>
#include <QMouseEvent>
#include "picpreview.h"
#include "pictureshow.h"

class PicPreConfig : public QWidget
{
    Q_OBJECT
public:
    explicit PicPreConfig(QWidget *parent = 0);
    ~PicPreConfig();
    void mouseMoveEvent( QMouseEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void set_view( PicPreview * pic_pre_view, QFrame *move_frame, QString* ps_image_name, QList<PicPreviewShowInfo> *p_show_list_info );
    void set_frame( QMouseEvent *event );
signals:

public slots:

private:
    PicPreview *mp_pic_view;
    QPointF m_pt_press_global;
    QString ms_move_image_name;
    QFrame *mp_move_frame;
    QString *mp_str_image_name;
    QList<PicPreviewShowInfo> *mp_list_show_info;

};

#endif // PICPRECONFIG_H

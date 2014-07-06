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

typedef struct {
    QImage *p_image;
    QString str_path;
    QString str_filename;//
    QString str_sn;
} PicPreviewInfo;

class PicPreview : public QFrame
{
    Q_OBJECT
public:
    explicit PicPreview(QWidget *parent = 0);
    ~PicPreview();

    void paintEvent( QPaintEvent *event );
    void resizeEvent( QResizeEvent *event );
    void mousePressEvent( QMouseEvent *event );
    QList<PicPreviewInfo> *get_list_value();

    int get_scroll_len();
    bool get_press_status();
    bool set_press_status( bool b ) { mb_left_btn_press = b; }
    QString *get_select_image_name();
signals:

public slots:
    void set_line_edit_value( int value );
    void set_frame( const QPointF *p_point );

    void delete_image();
    void delete_image( QString str_sn );
    int add_image( QString str_file_path, QString str_sn );

    void slot_add_image();
private:
    QImage *mp_image;
    QImage *mp_image1;
    QImage *mp_image2;
    QImage *mp_image3;
    QImage *mp_image4;

    QScrollBar *mp_scrollBar;
    int mn_len;
    QPushButton *mp_erase;
    QPushButton *mp_add_button;

    QPointF m_pt_press_local;
    bool mb_left_btn_press;
    //int ms_image_name;//图片位置
    QString ms_image_name;

    QList< PicPreviewInfo > m_list_pic_preview;
};

#endif // PICPREVIEW_H

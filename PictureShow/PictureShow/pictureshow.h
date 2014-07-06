#ifndef PICTURESHOW_H
#define PICTURESHOW_H

#include <QMainWindow>
#include <QImage>
#include <QSlider>
#include <QList>

typedef struct {
    QImage *p_image;
    QString str_path;
    QString str_filename;//
    QString str_sn;
} PicPreviewShowInfo;

namespace Ui {
class PictureShow;
}

class PictureShow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PictureShow(QWidget *parent = 0);
    ~PictureShow();

public:
    void paintEvent( QPaintEvent *event );
    void resizeEvent( QResizeEvent *event );
    QList<PicPreviewShowInfo> *get_show_list_value();

public slots:
    void setLineEditValue(int value);
private:
    Ui::PictureShow *ui;
    QImage *mp_image;
    QSlider *mp_slider;
    QList<PicPreviewShowInfo> m_list_show_frame;
};

#endif // PICTURESHOW_H

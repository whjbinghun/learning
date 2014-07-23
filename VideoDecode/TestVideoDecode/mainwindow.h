#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qimage.h>
#include "../video_decode_lib/video_decode_api.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void paintEvent(QPaintEvent *event);
    void callback_video_decode(unsigned char *puc_out_buf, unsigned int ui_data_len, int width, int height, int num, int den);

private slots:
    void on_id_btn_open_clicked();

private:
    Ui::MainWindow *ui;

private:
    YHANDLE  mh_video_decode;
    QImage m_image;
};

int callback_video_decode_out_data (unsigned char *puc_out_buf, unsigned int ui_data_len, int width, int height, int num, int den);

#endif // MAINWINDOW_H

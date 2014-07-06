#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include "YFNetCaptureEx.h"
#include "../YFIR/IRStruct.h"
#include <QDebug>
#include <string>

using namespace std;

namespace Ui {
class MainWindow;
}

void on_receive_corrected( void *p_user_data, unsigned short *pw_ad, unsigned short w_width, unsigned short w_height, DataStatus status);
void cb_bits_ready( void *p_user_data, YHANDLE h );

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent( QPaintEvent *event );
    bool event(QEvent *event);

private slots:
    void on_id_btn_connect_clicked();
    void on_id_btn_search_clicked();

private:
    Ui::MainWindow *ui;

    YHANDLE mh_discover;
    YHANDLE mh_net_capture;
    unsigned short mw_width, mw_height;

    QImage m_image;

    unsigned short get_width(){ return mw_width; }
    unsigned short get_height(){ return mw_height; }

    void draw_anas();
    void draw_anas_text( QPainter *p_draw );
    void draw_mac_srial( QPainter *p_draw );

public:
    void update_show(YHANDLE h);

public:
    static bool on_discover( const string &s_name, const string &s_ip, void *p_user_data );
    bool on_discover_real( const string &s_name, const string &s_ip );
};

#endif // MAINWINDOW_H

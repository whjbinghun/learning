#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qfiledialog.h>
#include <qpainter.h>
#include <qiodevice.h>
#include <qfile.h>
#include <qdebug.h>

MainWindow *g_p_main_window;
//
int callback_video_decode_out_data(unsigned char *puc_out_buf, unsigned int ui_data_len, int width, int height, int num, int den)
{
    if ( g_p_main_window != NULL ) {
        g_p_main_window->callback_video_decode( puc_out_buf, ui_data_len, width, height, num, den );
    }
}

void MainWindow::callback_video_decode(unsigned char *puc_out_buf, unsigned int ui_data_len, int width, int height, int num, int den)
{
    memcpy( m_image.bits(), puc_out_buf,  ui_data_len );

    update();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    g_p_main_window = this;
    m_image = QImage( 1920, 1080, QImage::Format_RGBA8888 );
    video_decode_init( &mh_video_decode, 0,  callback_video_decode_out_data );
}

MainWindow::~MainWindow()
{
   video_decode_deinit(  &mh_video_decode );
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter draw;
    draw.begin( this );

    if( m_image.isNull() ){
        draw.fillRect( 0, 0, 640, 480, QBrush( QColor( 200, 200, 200 ) ) );
    } else {
        draw.drawImage( QRect(0,0,640,480), m_image );
    }

    draw.end();
}

void MainWindow::on_id_btn_open_clicked()
{
    QString str_path = QFileDialog::getOpenFileName( this, tr("打开解码文件"), ".", tr("解码文件(*.mp4)") );
    if ( str_path.length() == 0 ) return;

    QFile file(str_path);
    if ( !file.open( QIODevice::ReadOnly ) ) return;

    char by_buff[4096];
    int n_read_len = file.read( by_buff, 4096 );
    while ( n_read_len > 0 ) {
        video_put_data( mh_video_decode, (unsigned char *)by_buff, n_read_len );
        n_read_len = file.read( by_buff, 4096 );
    }

}

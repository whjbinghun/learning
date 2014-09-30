#include "dialogtitlebar.h"
#include "ui_dialogtitlebar.h"
#include <QPainter>

#include <QDebug>

#define TITLE_BAR_HEIGHT 70

DialogTitleBar::DialogTitleBar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTitleBar)
    ,mp_title_bar( NULL )
    ,mp_login_dialog( NULL )
{
    ui->setupUi(this);
    init_title_bar();
    init_login_dialog();
}

DialogTitleBar::~DialogTitleBar()
{
    delete ui;
}

void DialogTitleBar::init_title_bar()
{
    //隐藏标题栏
    showFullScreen();
    setWindowFlags (Qt::FramelessWindowHint);
    //创建标题栏  类
    mp_title_bar = new TitleBar( this );
    mp_title_bar->resize( width(), TITLE_BAR_HEIGHT );
    mp_title_bar->setMouseTracking( true );
}

void DialogTitleBar::paintEvent(QPaintEvent *event)
{
    QPainter draw( this );
    //标题栏背景图片
    draw.drawPixmap( 0, 0, width(), TITLE_BAR_HEIGHT, QPixmap( ":image/res/caption_back.bmp" ) );
    draw.drawPixmap( 0, TITLE_BAR_HEIGHT, width(), height()-TITLE_BAR_HEIGHT, QPixmap( ":image/res/background.bmp" ) );
    draw.end();

}

void DialogTitleBar::init_login_dialog()
{
    mp_login_dialog = new LoginDialog(  );
    mp_login_dialog->setParent( this );
    mp_login_dialog->show();
    int n_height = height()-TITLE_BAR_HEIGHT;

    mp_login_dialog->move( (width()-mp_login_dialog->width())/2, (n_height-mp_login_dialog->height())/2 );
    //mp_login_dialog->resize( DIALOG_WIDTH, DIALOG_HEIGHT );
}

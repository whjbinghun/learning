#include "dialogtitlebar.h"
#include "ui_dialogtitlebar.h"
#include <QPainter>

#include <QDebug>

#define TITLE_BAR_HEIGHT 70
#define DIALOG_WIDTH 200
#define DIALOG_HEIGHT 150

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
    draw.drawPixmap( 0, 0, width(), TITLE_BAR_HEIGHT, QPixmap( ":image/res/background.bmp" ) );
    draw.drawPixmap( 0, TITLE_BAR_HEIGHT, width(), height()-TITLE_BAR_HEIGHT, QPixmap( ":image/res/background.bmp" ) );
    draw.end();

}

void DialogTitleBar::init_login_dialog()
{
    mp_login_dialog = new LoginDialog( this );
    mp_login_dialog->setParent( this );
    mp_login_dialog->show();
    int n_height = height()-TITLE_BAR_HEIGHT;

    mp_login_dialog->move( (width()-DIALOG_WIDTH)/2, (n_height-DIALOG_HEIGHT)/2 );
    mp_login_dialog->resize( DIALOG_WIDTH, DIALOG_HEIGHT );
}

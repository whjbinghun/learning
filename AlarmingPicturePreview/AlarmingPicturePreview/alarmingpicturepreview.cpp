#include "alarmingpicturepreview.h"
#include "ui_alarmingpicturepreview.h"
#include <QPainter>

#define TITLE_BAR_HEIGHT 70
#define BTN_WIDTH 75
#define BTN_HEIGHT 24
#define TAB_WIDTH 300
#define SPACE 20
#define PAN_RAIL_SPACE 30
#define DEL_BTN_WIDTH 100


AlarmingPicturePreview::AlarmingPicturePreview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlarmingPicturePreview)
    ,mp_timer_heartbeat( NULL )
   // ,mp_title_bar( NULL )
{
    ui->setupUi(this);
    init_title_bar();
    init_list_font();

    mp_timer_heartbeat = new QTimer(this);
    connect( mp_timer_heartbeat, SIGNAL( timeout() ), this, SLOT( slot_timer_heart_beat() ) );

    /*ui->id_calendar_widget->setStyleSheet( "border: 1px solid red;");
    ui->id_frame_ir->setStyleSheet( "border: 1px solid red;");
    ui->id_frame_vi->setStyleSheet( "border: 1px solid red;");
    ui->id_frame_image->setStyleSheet( "border: 1px solid red;");
    ui->id_list_group->setStyleSheet( "border: 1px solid red;");
    ui->id_tablewidget_preset_alarm_list->setStyleSheet( "border: 1px solid red;");
    ui->id_widget_time->setStyleSheet( "border: 1px solid red;");
    */
}

AlarmingPicturePreview::~AlarmingPicturePreview()
{
    delete ui;
}

void AlarmingPicturePreview::paintEvent( QPaintEvent *p_event )
{
    QPainter draw;
    draw.begin( this );
    draw.drawImage( QRect( 0, TITLE_BAR_HEIGHT, width(), height()-TITLE_BAR_HEIGHT ), QImage( ":image/image/caption_background.bmp" ) );
    draw.end();
}

void AlarmingPicturePreview::resizeEvent( QResizeEvent *p_event )
{
    move_widget();
}

void AlarmingPicturePreview::move_widget()
{
    int n_width = width();
    int n_height = height()-TITLE_BAR_HEIGHT;
    QRect rct_list, rct_ir_vi, rct_alarm, rct_preset_list;
    rct_list = QRect( SPACE, TITLE_BAR_HEIGHT+SPACE, TAB_WIDTH, n_height-2*SPACE );
    int n_ir_vi_width = n_width-SPACE*3-TAB_WIDTH;
    int n_ir_vi_height = n_ir_vi_width*9/28;
    rct_ir_vi = QRect( SPACE*2+TAB_WIDTH, TITLE_BAR_HEIGHT+SPACE,  n_ir_vi_width, n_ir_vi_height );
    rct_alarm = QRect( SPACE*2+TAB_WIDTH, TITLE_BAR_HEIGHT+SPACE*2+n_ir_vi_height, n_ir_vi_width, SPACE*2 );
    rct_preset_list = QRect( SPACE*2+TAB_WIDTH, TITLE_BAR_HEIGHT+SPACE*5+n_ir_vi_height, n_ir_vi_width, n_height-SPACE*6-n_ir_vi_height );

    move_list( rct_list );
    move_ir_vi( rct_ir_vi );
    move_alarm( rct_alarm );
    move_preset_list( rct_preset_list );
}

void AlarmingPicturePreview::move_list( QRect rct )
{
    int n_one_height = (rct.height()-2*SPACE)/8;
    ui->id_label_group_list->move( rct.x(), rct.y() );
    ui->id_list_group->move( rct.x(), rct.y()+ui->id_label_group_list->height() );
    ui->id_list_group->resize( rct.width(), n_one_height*2-ui->id_label_group_list->height() );
    ui->id_frame_image->move( rct.x(), rct.y()+n_one_height*2 +SPACE );
    ui->id_frame_image->resize( rct.width(), n_one_height*4 );
    ui->id_calendar_widget->move( rct.x(), rct.y()+n_one_height*6+2*SPACE );
    ui->id_calendar_widget->resize( rct.width(), rct.height()-n_one_height*6-SPACE*2 );
}

void AlarmingPicturePreview::move_ir_vi( QRect rct )
{
    int n_ir_width = rct.width()*12/28;
    int n_vi_width = rct.width()*16/28;
    ui->id_frame_ir->move( rct.x(), rct.y() );
    ui->id_frame_ir->resize( n_ir_width, rct.height() );
    ui->id_frame_vi->move( rct.x()+n_ir_width, rct.y() );
    ui->id_frame_vi->resize( n_vi_width, rct.height() );
}

void AlarmingPicturePreview::move_alarm( QRect rct )
{
    ui->id_widget_time->move( rct.x(), rct.y() );
    ui->id_widget_time->resize( rct.width(), rct.height() );
}

void AlarmingPicturePreview::move_preset_list( QRect rct )
{
    ui->id_tablewidget_preset_alarm_list->move( rct.x(), rct.y() );
    ui->id_tablewidget_preset_alarm_list->resize( rct.width(), rct.height() );
}

void AlarmingPicturePreview::init_title_bar() //标题栏初始化
{
    showFullScreen();
    setWindowFlags( Qt::FramelessWindowHint ); //隐藏标题栏
    //mp_title_bar = new TitleBar( this );
    //mp_title_bar->resize( ui->centralWidget->width(), TITLE_BAR_HEIGHT );
    //mp_title_bar->setMouseTracking( true );

    setMinimumWidth(850);
    setMinimumHeight(600);
}

void AlarmingPicturePreview::init_list_font()
{
    QFont ft;
    ft.setPointSize(20);
    ui->id_list_group->setFont( ft );
}

void AlarmingPicturePreview::init_group_num()
{
    /*QMap< int, DeviceGroup > map_group;
    CDataMode::Instance()->get_group_device( map_group );//组号，组信息

    int i=0;
    QMap< int, DeviceGroup >::iterator it = map_group.begin();

    for( ; it != map_group.end(); ++it ) {
        ui->id_list_group->addItem( new QListWidgetItem( "组号-"+QString::number( it.key() ) ) );
    }

    //slot_timer_heart_beat();
    mp_timer_heartbeat->start( 60000 );*/
}

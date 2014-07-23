#include "demo.h"
#include "ui_demo.h"
#include <QToolButton>
#include <QHBoxLayout>
#include <QListView>
#include <QTextEdit>
#include <QScrollArea>
#include <QDebug>

#include <QHostAddress>
#include <QFile>
#include <QMessageBox>
#include <QDomDocument>
#include <QDateTime>
#include <QTimer>
#include <QImage>
#include <QFrame>
#include <QtEndian>
#include <QFileDialog>


//序列号
static  unsigned int uint_seq_id = 0;

#define CONNECT_NUMBER_MAX  3
#define WIDTH_LIST_VIEW_DEVICE  180              //160表示listView所占用的宽度
#define SPACE 20        //各控件的间隔

//云台控制窗口的长度和宽度
#define WIDTH_PLAYFORM 350
#define HEIGHT_PLAYFORM 250

//
int callback_ir_decode_out_data(unsigned char *puc_out_buf, unsigned int ui_data_len, int width, int height, void *p_user_data)
{
    Demo *p_this = ( Demo * )p_user_data;
    if ( p_this != NULL ) {
        p_this->callback_ir_decode( puc_out_buf, ui_data_len, width, height );
    }
    return 0;
}

void Demo::callback_ir_decode(unsigned char *puc_out_buf, unsigned int ui_data_len, int width, int height )
{
    int n_start = QTime::currentTime().msecsSinceStartOfDay();
    if ( m_mutex_ir.tryLock() ) {
        if ( mn_ir_connect_status == YF_Disconnected ) {
            m_ir_image.fill( QColor(200,200,200) );
        } else {
            memcpy( m_ir_image.bits(), puc_out_buf, ui_data_len );
        }
        m_mutex_ir.unlock();
    }
    //qDebug() << "callback_ir_decode" << QTime::currentTime().msecsSinceStartOfDay() - n_start;
}


int callback_vi_decode_out_data(unsigned char *puc_out_buf, unsigned int ui_data_len, int width, int height, void *p_user_data)
{
    Demo *p_this = ( Demo * )p_user_data;
    if ( p_this != NULL ) {
        p_this->callback_vi_decode( puc_out_buf, ui_data_len, width, height );
    }
    return 0;
}

void Demo::callback_vi_decode(unsigned char *puc_out_buf, unsigned int ui_data_len, int width, int height)
{
    int n_start = QTime::currentTime().msecsSinceStartOfDay();
    if ( m_mutex_vi.tryLock() ) {
        if ( mn_vi_connect_status == YF_Disconnected ) {
            m_vi_image.fill( QColor(200,200,200) );
        } else {
            memcpy( m_vi_image.bits(), puc_out_buf,  ui_data_len );
        }
        m_mutex_vi.unlock();
    }

    //qDebug() << "callback_vi_decode"<< QTime::currentTime().msecsSinceStartOfDay() - n_start;
}

Demo::Demo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Demo)
  , mp_tcp_client(0)
  , mp_ir_tcp_client(0)
  , mp_vi_tcp_client(0)
  , mn_ir_connect_status( YF_Disconnected )
  , mn_vi_connect_status( YF_Disconnected )
 {
    ui->setupUi(this);

    init_tool_bar();    //工具栏初始化
    init_mouse();       //鼠标初始化
    init_vi_show();
    init_ir_show();
    init_xml_buf();
    init_connect();
    init_device_list();
    init_timer();

    qDebug()<<m_ir_image.width()<<m_ir_image.height()<<m_vi_image.width()<<m_vi_image.height();
    ui->id_textedit_device->hide();
    ui->id_frame_ir->hide();
    ui->id_frame_vi->hide();
}

Demo::~Demo()
{
    if ( mp_tcp_client && mp_tcp_client->state() != QTcpSocket::UnconnectedState ) {
        mp_tcp_client->disconnectFromHost();
        mp_tcp_client->waitForDisconnected();
    }

    if ( mp_ir_tcp_client && mp_ir_tcp_client->state() != QTcpSocket::UnconnectedState ) {
        mp_ir_tcp_client->disconnectFromHost();
        mp_ir_tcp_client->waitForDisconnected();
    }

    if ( mp_vi_tcp_client && mp_vi_tcp_client->state() != QTcpSocket::UnconnectedState ) {
        mp_vi_tcp_client->disconnectFromHost();
        mp_vi_tcp_client->waitForDisconnected();
    }

    video_decode_deinit(  &mh_vi_video_decode );
    video_decode_deinit( &mh_ir_video_decode );
    delete ui;
}

void Demo::init_device_list()
{
    mp_standard_item_model = new QStandardItemModel(this);

    ui->id_listview_device->setModel( mp_standard_item_model );
    ui->id_listview_back_device->setModel( mp_standard_item_model );

    //connect(ui->id_listview_device,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(itemClicked(QModelIndex)));
    //connect(ui->id_listview_back_device, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
}

void Demo::init_timer()
{
    //创建定时器
    QTimer *heart_timer = new QTimer(this);
    //将定时器超时信号与槽(功能函数)联系起来
    connect( heart_timer,SIGNAL(timeout()), this, SLOT(send_heart_request()) );
    //开始运行定时器，定时时间间隔为1000ms
    heart_timer->start(1000*60);


    QTimer *p_update_timer = new QTimer(this);
    connect( p_update_timer,SIGNAL(timeout()), this, SLOT(update_show()) );
    p_update_timer->start(40);
    //如果m_vi_play_buf列表头不为空，则等待40ms，传输一帧数据，并删除列表头的数据
    //QTimer *vi_play_timer = new QTimer( this );
    //connect( vi_play_timer, SIGNAL( timeout() ), this, SLOT( set_vi_play() ) );
    //vi_play_timer->start( 20 );
}

void Demo::init_xml_buf()
{
    //xml文件缓冲区
    mp_xml_buf = new char[2048];
    memset( mp_xml_buf, 0, 2048 );
}

void Demo::init_vi_show()
{
    m_vi_buf_len = FRAME_VI_SIZE*2;
    m_vi_image = QImage( 1920, 1080, QImage::Format_RGBA8888);
    m_vi_image.fill( QColor( 200, 200, 200 ) );

    m_vi_buf = new char[ m_vi_buf_len ];
    memset( m_vi_buf, 0, m_vi_buf_len );

    video_decode_init( &mh_vi_video_decode, 0,  callback_vi_decode_out_data, this );
}

void Demo::init_ir_show()
{
    m_ir_buf_len = FRAME_SIZE*4;
    m_ir_image = QImage( 640, 480, QImage::Format_RGBA8888);
    m_ir_image.fill( QColor( 200, 200, 200 ) );
    m_ir_buf = new char[m_ir_buf_len];
    memset( m_ir_buf, 0, m_ir_buf_len );

    video_decode_init( &mh_ir_video_decode, 0, callback_ir_decode_out_data, this );
}

void Demo::init_tool_bar() {
    mp_button_new = new QToolButton;
    mp_button_open = new QToolButton;
    mp_button_new->setText("显示窗口");
    mp_button_open->setText("回放模块");
    ui->id_main_tool_bar->addWidget(mp_button_new);
    ui->id_main_tool_bar->addWidget(mp_button_open);
    mp_button_open->setStyleSheet("color:black");
    mp_button_new->setStyleSheet("color:red");

    ui->stackedWidget->addWidget(ui->play_mode);
    ui->stackedWidget->addWidget(ui->back_mode);

    connect(mp_button_new, SIGNAL(clicked()), this, SLOT(play_show()));
    connect(mp_button_open,SIGNAL(clicked()), this, SLOT(back_show()));
}

void Demo::init_connect()
{
    //在头文件中定义指针，必须初始化
    mp_tcp_client = new QTcpSocket(this);
    mp_ir_tcp_client = new QTcpSocket(this);
    mp_vi_tcp_client = new QTcpSocket(this);

    connect(mp_tcp_client, SIGNAL(connected()), this, SLOT(connect_sucessful()));   //服务器链接成功
    connect(mp_tcp_client, SIGNAL(readyRead()), this, SLOT(receiver_message()));    //接收服务器发送的数据
    connect(mp_tcp_client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));

    connect(mp_vi_tcp_client, SIGNAL(readyRead()), this, SLOT(receiver_vi_play_message()));
    connect(mp_ir_tcp_client, SIGNAL(readyRead()), this, SLOT(receiver_ir_play_message()));

    connect_server();
}

void Demo::connect_server()
{
    mn_blockSize = 0; //初始化其为0
    mp_tcp_client->abort(); //取消已有的连接
    //mp_tcp_client->connectToHost(QHostAddress::LocalHost,(quint16)5555);
    mp_tcp_client->connectToHost("192.168.0.200", 5555);
}

void Demo::play_show()
{
    mp_button_open->setStyleSheet("color:black");
    mp_button_new->setStyleSheet("color:red");
    ui->stackedWidget->setCurrentIndex(0);
}

void Demo::back_show()
{
    QString str_path = QFileDialog::getOpenFileName( this, tr("打开解码文件"), ".", tr("解码文件(*.mp4)") );
    if ( str_path.length() == 0 ) return;

    QFile file(str_path);
    if ( !file.open( QIODevice::ReadOnly ) ) return;

    char by_buff[4096];
    int n_read_len = file.read( by_buff, 4096 );
    while ( n_read_len > 0 ) {
        video_put_data( mh_vi_video_decode, (unsigned char *)by_buff, n_read_len );
        n_read_len = file.read( by_buff, 4096 );
    }

    file.close();
}
/*
void Demo::back_show(){
    mp_button_new->setStyleSheet("color:black");
    mp_button_open->setStyleSheet("color:red;");
    ui->stackedWidget->setCurrentIndex(1);
}*/

//窗体布局
void Demo::resizeEvent( QResizeEvent * event ) {

    int n_width = ui->centralWidget->width() - SPACE * 2 - WIDTH_LIST_VIEW_DEVICE;
    int n_height = ui->centralWidget->height() - SPACE * 2;
    //qDebug()<<n_width<<n_height;

    ui->stackedWidget->move( 0, 0 );
    ui->stackedWidget->resize( ui->centralWidget->width(), ui->centralWidget->height() );


    //160表示listView_3所占用的宽度  frame的宽：高=4：3；
    if(n_width>=n_height*8/5){
        //play_mode
        //设备列表
        ui->id_listview_device->move( SPACE, SPACE );
        ui->id_listview_device->resize( WIDTH_LIST_VIEW_DEVICE, n_height );
        //红外播放
        ui->id_frame_ir->move( WIDTH_LIST_VIEW_DEVICE + SPACE, SPACE );
        ui->id_frame_ir->resize( n_height*4/5, n_height*3/5 );

        //可见光播放
        ui->id_frame_vi->move(WIDTH_LIST_VIEW_DEVICE+n_height*4/5+SPACE,SPACE);
        ui->id_frame_vi->resize(n_height*4/5,n_height*3/5);
        //qDebug()<<"3333"<<n_height*8/5<< n_height;

        //ui->id_textedit_device->move(WIDTH_LIST_VIEW_DEVICE+SPACE,n_height*3/5+SPACE);
        //ui->id_textedit_device->resize(n_height*4/5,n_height*2/5);

        ui->id_label->move( WIDTH_LIST_VIEW_DEVICE+SPACE+10, n_height - 220 );
        ui->id_line_edit->move( WIDTH_LIST_VIEW_DEVICE+SPACE+10+50+10, n_height - 220 );
        ui->id_pushbutton_set->move( WIDTH_LIST_VIEW_DEVICE+SPACE+10, n_height - 170 );
        ui->id_pushbutton_recall->move( WIDTH_LIST_VIEW_DEVICE+SPACE+10, n_height - 110  );
        ui->id_pushbutton_cruise->move( WIDTH_LIST_VIEW_DEVICE+SPACE+10, n_height - 50 );

        ui->id_pushbutton_up->move( WIDTH_LIST_VIEW_DEVICE+SPACE+10+190, n_height - 170  );
        ui->id_pushbutton_left->move( WIDTH_LIST_VIEW_DEVICE+SPACE+10+120, n_height - 110 );
        ui->id_pushbutton_right->move( WIDTH_LIST_VIEW_DEVICE+SPACE+10+120+70+70, n_height - 110 );
        ui->id_pushbutton_down->move( WIDTH_LIST_VIEW_DEVICE+SPACE+10+190, n_height - 50 );

        //红外温度分析
        //ui->id_frame_ana->move(WIDTH_LIST_VIEW_DEVICE+n_height*4/5+SPACE,n_height*3/5+SPACE);
        //ui->id_frame_ana->resize(n_height*4/5,n_height*2/5);
        ui->id_frame_ana->move( WIDTH_LIST_VIEW_DEVICE+SPACE+WIDTH_PLAYFORM, n_height*3/5 + SPACE );
        ui->id_frame_ana->resize( n_height*8/5 - WIDTH_PLAYFORM, n_height*2/5 );

        //back_mode
        ui->id_listview_back_device->move(SPACE,SPACE);
        ui->id_listview_back_device->resize(WIDTH_LIST_VIEW_DEVICE,n_height*3/5);
        ui->id_frame_back_ir->move(WIDTH_LIST_VIEW_DEVICE+SPACE,SPACE);
        ui->id_frame_back_ir->resize(n_height*4/5,n_height*3/5);
        ui->id_frame_back_vi->move(WIDTH_LIST_VIEW_DEVICE+SPACE+n_height*4/5,SPACE);
        ui->id_frame_back_vi->resize(n_height*4/5,n_height*3/5);

        ui->id_text_browser_device->move(SPACE,SPACE+n_height*3/5);
        ui->id_text_browser_device->resize(WIDTH_LIST_VIEW_DEVICE,n_height*2/5);
        ui->id_frame_back_ana->move(WIDTH_LIST_VIEW_DEVICE+SPACE,SPACE+n_height*3/5);
        ui->id_frame_back_ana->resize(n_height*8/5,n_height*2/5);

    }else{
        //play_mode
        ui->id_listview_device->move(SPACE,SPACE);
        ui->id_listview_device->resize(WIDTH_LIST_VIEW_DEVICE,n_height);
        ui->id_frame_ir->move( WIDTH_LIST_VIEW_DEVICE+SPACE, SPACE );
        ui->id_frame_ir->resize( n_width/2, n_width*3/8 );

        ui->id_frame_vi->move(WIDTH_LIST_VIEW_DEVICE+n_width/2+SPACE,SPACE);
        ui->id_frame_vi->resize(n_width/2,n_width*3/8);
        //qDebug()<<"4444"<<n_width<<n_width*3/8;

        //ui->id_textedit_device->move(WIDTH_LIST_VIEW_DEVICE+SPACE,n_width*3/8+SPACE);
        //ui->id_textedit_device->resize(n_width/2, n_height-n_width*3/8);

        ui->id_label->move( WIDTH_LIST_VIEW_DEVICE+SPACE+10, n_height - 220 );
        ui->id_line_edit->move( WIDTH_LIST_VIEW_DEVICE+SPACE+10+10+50, n_height - 220 );
        ui->id_pushbutton_set->move( WIDTH_LIST_VIEW_DEVICE+SPACE+10, n_height - 170 );
        ui->id_pushbutton_recall->move( WIDTH_LIST_VIEW_DEVICE+SPACE+10, n_height - 110  );
        ui->id_pushbutton_cruise->move( WIDTH_LIST_VIEW_DEVICE+SPACE+10, n_height - 50 );

        ui->id_pushbutton_up->move( WIDTH_LIST_VIEW_DEVICE+SPACE+10+190, n_height - 170  );
        ui->id_pushbutton_left->move( WIDTH_LIST_VIEW_DEVICE+SPACE+10+120, n_height - 110 );
        ui->id_pushbutton_right->move( WIDTH_LIST_VIEW_DEVICE+SPACE+10+120+140, n_height - 110 );
        ui->id_pushbutton_down->move( WIDTH_LIST_VIEW_DEVICE+SPACE+10+190, n_height - 50 );


        //ui->id_frame_ana->move(WIDTH_LIST_VIEW_DEVICE+n_width/2+SPACE,n_width*3/8+SPACE);
        //ui->id_frame_ana->resize(n_width/2, n_height-n_width*3/8);
        ui->id_frame_ana->move( WIDTH_LIST_VIEW_DEVICE+SPACE+WIDTH_PLAYFORM, n_width*3/8 + SPACE );
        ui->id_frame_ana->resize( n_width - WIDTH_PLAYFORM, n_height-n_width*3/8 );


        //back_mode
        ui->id_listview_back_device->move(SPACE,SPACE);
        ui->id_listview_back_device->resize(WIDTH_LIST_VIEW_DEVICE,n_width*3/8);

        ui->id_frame_back_ir->move(WIDTH_LIST_VIEW_DEVICE+SPACE,SPACE);
        ui->id_frame_back_ir->resize(n_width/2,n_width*3/8);
        ui->id_frame_back_vi->move(WIDTH_LIST_VIEW_DEVICE+SPACE+n_width/2,SPACE);
        ui->id_frame_back_vi->resize(n_width/2,n_width*3/8);

        ui->id_text_browser_device->move(SPACE,SPACE+n_width*3/8);
        ui->id_text_browser_device->resize(WIDTH_LIST_VIEW_DEVICE,n_height-n_width*3/8);
        ui->id_frame_back_ana->move(WIDTH_LIST_VIEW_DEVICE+SPACE,SPACE+n_width*3/8);
        ui->id_frame_back_ana->resize(n_width,n_height-n_width*3/8);
    }

   // update();
}

void Demo::init_mouse()
{
    ui->id_listview_device->setMouseTracking(true);
    ui->id_frame_ir->setMouseTracking(true);
    ui->id_frame_vi->setMouseTracking(true);
    ui->id_textedit_device->setMouseTracking(true);
    ui->id_frame_ana->setMouseTracking(true);

    ui->id_listview_back_device->setMouseTracking(true);
    ui->id_frame_back_ir->setMouseTracking(true);
    ui->id_frame_back_vi->setMouseTracking(true);
    ui->id_text_browser_device->setMouseTracking(true);
    ui->id_frame_back_ana->setMouseTracking(true);
}

void Demo::connect_sucessful()
{
    qDebug()<<"链接成功";
    send_xml_request();    //向服务器发送xml请求
}

//向服务器发送xml请求
void Demo::send_xml_request()
{
    XmlReq xml_req;
    xml_req.header.msg_type = qToBigEndian((uint)IR_MONITOR_XML_DATA_REQ);

    //应答消息流水号
    ++uint_seq_id ;
    m_seq_id_list<<QString::number(uint_seq_id);

    xml_req.header.seq_id = qToBigEndian(uint_seq_id);
    xml_req.client_id =qToBigEndian(1);
    int n_xml_length = sizeof(xml_req);
    xml_req.header.total_len = qToBigEndian(n_xml_length);

    memcpy( mp_xml_buf, &xml_req, n_xml_length );
    mp_tcp_client->write( mp_xml_buf, n_xml_length );
}

//播放视频 请求
void Demo::connect_play( QString str_device_sn, int n_class )
{
    qDebug()<<"请求连接:"<<str_device_sn << QString(n_class);

    DevicePlayInfo device_play_info;
    VodPlayReq vod_play_req;

    device_play_info.n_class = n_class;
    device_play_info.seq_id = ++uint_seq_id;

    vod_play_req.header.msg_type = qToBigEndian((uint)IR_MONITOR_VID_DATA_PLAY_REQ);
    vod_play_req.header.seq_id = qToBigEndian(uint_seq_id);

    int n_len = sizeof( vod_play_req.header )+strlen( str_device_sn.toLatin1().data() )+1;
    vod_play_req.header.total_len = qToBigEndian(n_len);

    memcpy( vod_play_req.client_id, str_device_sn.toLatin1().data(), strlen( str_device_sn.toLatin1().data() )+1 );//s_list_sn_seq_id[0].toLatin1().data();

    //加载seq_id 和 n_class
    m_seq_id_class_list.append(QString::number(device_play_info.seq_id).append(",").append(QString::number(device_play_info.n_class)));

    qDebug()<<"device_play_info:"<<device_play_info.n_class<<uint_seq_id<<str_device_sn;
    qDebug()<<"播放视频请求:"<<uint_seq_id<<vod_play_req.client_id<<n_len;

    mp_tcp_client->write((char *)&vod_play_req, n_len);
}

void Demo::disconnect_play( QString str_device_sn, int n_class )
{
    VodStopPlayReq vod_stop_play_req;
    DevicePlayInfo device_play_info;

    device_play_info.n_class = n_class;
    device_play_info.seq_id = ++uint_seq_id;

    vod_stop_play_req.header.msg_type = qToBigEndian((uint)IR_MONITOR_VID_DATA_STOP_REQ);

    m_stop_seq_id_class_list.append(QString::number(device_play_info.seq_id).append(",").append(QString::number(device_play_info.n_class)));
    qDebug()<<"stop 请求"<<m_stop_seq_id_class_list;

    vod_stop_play_req.header.seq_id = qToBigEndian(uint_seq_id);
    //设备名
    memcpy( vod_stop_play_req.client_id, str_device_sn.toLatin1().data(), strlen( str_device_sn.toLatin1().data() )+1 );
    int n_len = sizeof(vod_stop_play_req.header) + strlen(str_device_sn.toLatin1().data())+1;
    vod_stop_play_req.header.total_len = qToBigEndian(n_len);

    qDebug()<<vod_stop_play_req.header.msg_type<< vod_stop_play_req.header.seq_id<<vod_stop_play_req.client_id<<vod_stop_play_req.header.total_len;

    mp_tcp_client->write((char *)&vod_stop_play_req, n_len);
}

//发送心跳检测请求
void Demo::send_heart_request()
{
    HeartBeatReq heart_beat_req;
    //htonl是主机字节序到网络字节序的转换
    heart_beat_req.msg_type = qToBigEndian((uint)IR_MONITOR_LINK_HEART_REQ);
    //应答消息流水号
    uint_seq_id = uint_seq_id + 1;
    heart_beat_req.seq_id = qToBigEndian(uint_seq_id);
    int n_heart_length = sizeof(heart_beat_req);
    heart_beat_req.total_len = qToBigEndian(n_heart_length);
    qDebug()<<heart_beat_req.msg_type<< heart_beat_req.seq_id<<heart_beat_req.total_len<<n_heart_length;
    char buf[1024];

    memcpy(buf, &heart_beat_req, n_heart_length);

    mp_tcp_client->write(buf, n_heart_length);
}

//接收 消息
void Demo::receiver_message()
{
    while(mp_tcp_client->bytesAvailable()>0)
    {
        int length = mp_tcp_client->bytesAvailable();

        char buf[1024*1024];
        //读取服务器的数据+
        mp_tcp_client->read(buf, length);

        //XmlResp xml_resp;
        MsgHeader msg_header;

        memcpy(&msg_header,buf,sizeof(msg_header));
        unsigned int msg_type;
        msg_type = qFromBigEndian(msg_header.msg_type);//32773
        switch (msg_type) {
        case IR_MONITOR_VER_NEGO_RESP :
            break;

        case IR_MONITOR_LINK_AUTH_RESP :
            break;

        case IR_MONITOR_LINK_HEART_RESP :
            qDebug()<<"心跳包回复成功!";
            break;

        case IR_MONITOR_LINK_REL_RESP :
            break;
        case IR_MONITOR_XML_DATA_RESP :  //xml 回复
            xml_reply(buf,length);
            break;

        case IR_MONITOR_VID_DATA_PLAY_RESP : //数据播放回复
            play_reply(buf, length);
            break;

        case IR_MONITOR_VID_DATA_STOP_RESP : //数据停止回复
            stop_play_reply(buf);
            break;

        case IR_MONITOR_MSG_TYPE_BUTT :
            break;
        default:
            break;
        }
    }
}

//接收视频
void Demo::receiver_play_message( char * url,  int n_class)
{
    QString str_url = url;
    QString str_ip = str_url.split(":")[0];
    int n_port = str_url.split(":")[1].toInt();
    QString str_port = str_url.split(":")[1];
    qDebug() << "视频IP，port" << url << str_ip << str_port << n_port;

    if( n_class == 0 ){
        mp_vi_tcp_client->connectToHost( str_ip, n_port);
        mn_vi_connect_status = YF_Connected;
    } else {
        mp_ir_tcp_client->connectToHost( str_ip, n_port);
        mn_ir_connect_status = YF_Connected;
    }
}

//接收红外视频数据流
void Demo::receiver_ir_play_message()
{
    if( mp_ir_tcp_client->bytesAvailable()>0 ) {
        quint64 n_data_len = mp_ir_tcp_client->bytesAvailable();
        if( n_data_len > m_ir_buf_len ) {
            delete []m_ir_buf;
            m_ir_buf = new char[ n_data_len ];
            m_ir_buf_len = n_data_len;
        }

        quint64 n_read_len = mp_ir_tcp_client->read( m_ir_buf, n_data_len );
        //qDebug()<<n_read_len;

        static int n_start_time = 0;
        int n_pass = QTime::currentTime().msecsSinceStartOfDay() - n_start_time;
        //qDebug()<< "红外间隔：" <<n_pass << n_data_len;
        n_start_time = QTime::currentTime().msecsSinceStartOfDay();


        int n_video_data = video_put_data( mh_ir_video_decode, (unsigned char *)m_ir_buf, n_read_len );
        //qDebug()<< "红外PUT："  << QTime::currentTime().msecsSinceStartOfDay() - n_start_time;
        if(n_video_data != 0 ) {
            qDebug()<<"数据读取出错";
        }

/*
#if 1
        quint64 length = mp_ir_tcp_client->bytesAvailable();
                //读取红外视频数据 m_ir_buf_len表示位移
                mp_ir_tcp_client->read(m_ir_buf + m_ir_buf_len, length);
                m_ir_buf_len += length;
                if( m_ir_buf_len >= FRAME_SIZE ) {
                    qDebug()<<"lengtH:"<<m_ir_buf_len<<m_ir_buf_len/(FRAME_SIZE);
                    //从m_ir_buf中拷贝数据到m_ir_image
                    memcpy( m_ir_image.bits(), m_ir_buf, FRAME_SIZE );
                    m_ir_buf_len -= FRAME_SIZE;
                    memcpy(m_ir_buf, m_ir_buf + FRAME_SIZE, FRAME_SIZE*3);
                    //刷新界面,然后画图事件被调用
                    this->update();
                }
#else
        QFile file("E:/monitor_file/monitor_files/client files/debug/debug/ir.rgb");
        static int frame_no = 0;
        if ( !file.open( QIODevice::ReadOnly ) ) {
            qDebug("read file err!");
            return;}

        char by_buff[FRAME_SIZE];
        while ( 1 ) {
            int n_read_len = file.read( by_buff, FRAME_SIZE );
            if ( n_read_len > 0 ) {
                memcpy( m_ir_image.bits(), by_buff, FRAME_SIZE );
                //QThread::msleep(40);
                this->update();
            } else {
                break;
            }
        }
#endif*/

        /*quint64 length = mp_ir_tcp_client->bytesAvailable();
        //quint64 length2 = mp_ir_tcp_client->read(m_ir_buf + m_ir_buf_len, length);
        quint64 r_len = mp_ir_tcp_client->read(m_ir_buf + m_ir_buf_len, length);
        //mp_ir_tcp_client->read(m_ir_buf + m_ir_buf_len, length);
        //m_ir_buf_len += length2;
        m_ir_buf_len += r_len;
        if( m_ir_buf_len >= FRAME_SIZE ) {
            qDebug()<<"lengtH:"<<m_ir_buf_len<<m_ir_buf_len/(FRAME_SIZE);
            //从m_ir_buf中拷贝数据到m_ir_image
            memcpy( m_ir_image.bits(), m_ir_buf, FRAME_SIZE );
#if 0
            QFile file("ir.rgb");
            static int frame_no = 0;
            file.open(QIODevice::WriteOnly|QIODevice::Append);
            if(frame_no++ < (25*15))
                file.write(m_ir_buf,FRAME_SIZE);
            else
                file.close();
#endif
            m_ir_buf_len -= FRAME_SIZE;
            memcpy(m_ir_buf, m_ir_buf + FRAME_SIZE, 3*FRAME_SIZE);
            //刷新界面,然后画图事件被调用
            this->update();
        }*/
    }
}

void Demo::receiver_vi_play_message()
{
    if(mp_vi_tcp_client->bytesAvailable()>0)
    {
        quint64 n_read_len = 0;
        quint64 n_data_len = mp_vi_tcp_client->bytesAvailable();
        //qDebug()<<length;
        //读取红外视频数据 m_ir_buf_len表示位移
        if( n_data_len > m_vi_buf_len ) {
            delete []m_vi_buf;
            m_vi_buf = new char[n_data_len];
            m_vi_buf_len = n_data_len;
        }

        static int n_start_time = 0;
        int n_pass = QDateTime::currentMSecsSinceEpoch() - n_start_time;
        //qDebug()<< "可见光间隔：" <<n_pass << n_data_len;
        n_start_time = QDateTime::currentMSecsSinceEpoch() ;

        n_read_len = mp_vi_tcp_client->read( m_vi_buf, n_data_len );
        int n_video_data = video_put_data( mh_vi_video_decode, (unsigned char *)m_vi_buf, n_read_len );
        if( n_video_data != 0 ) {
            qDebug()<<"数据读取出错";
        }
    }
}

void Demo::set_vi_play()
{
    int len = m_vi_play_buf.size();
    if( len > 0 ) {
        //qDebug()<<m_vi_play_buf[0].vi_buf<<m_vi_play_buf[0].len;
        memcpy( m_vi_image.bits(), m_vi_play_buf[0].vi_buf, m_vi_play_buf[0].len );
        //new vi_buf 需要delete才能释放数据。而removeAt只是说数据和列表失去关联
        delete []m_vi_play_buf[0].vi_buf;
        m_vi_play_buf.removeAt(0);
        this->update();
    }
}

void Demo::displayError(QAbstractSocket::SocketError)
{
    if( mp_tcp_client->errorString() == "Connection refused" ) {
        connect_server();
        qDebug() << "服务器链接失败";
    } else if( mp_tcp_client->errorString() == "The remote host closed the connection") {
        qDebug() << "远程服务器被关闭";
    }
}

void Demo::xml_reply(char * buf, int length){
    XmlResp xml_resp;

    //记录buf读取位置
    int n_pos=0;
    n_pos = sizeof(xml_resp.header.msg_type);
    n_pos = sizeof(xml_resp.header.total_len) + n_pos;
    memcpy(&xml_resp.header.seq_id,buf+n_pos, sizeof(xml_resp.header.seq_id));
    //遍历list，找到list中的值跟xml_resp.header.seq_id中的值是一样的，那么读取xml数据并删除list中的值
    //m_seq_id_list
    int n_count = m_seq_id_list.size();
    qDebug()<<"seq_id count:"<<n_count;
    qDebug()<<"开始 seq_id"<<m_seq_id_list;
    for(int i = 0; i < n_count; i++)
    {
        if( m_seq_id_list[i].toInt() == qFromBigEndian(xml_resp.header.seq_id) ){
            m_seq_id_list.removeAt(i);
            n_pos = sizeof(xml_resp.header);
            memcpy(&xml_resp.result,buf+n_pos,sizeof(xml_resp.result));
            if(xml_resp.result == 1) {
                n_pos = n_pos + sizeof(xml_resp.result);
                //memcpy(&xml_resp.xml_str,buf+13,length-13 );
                //创建文件
                QString s_file_name = "../monitor.xml";
                QFile file(s_file_name);
                //如果文件不存在，那就创建文件
                if(file.exists()==false) {
                    file.open(QIODevice::WriteOnly);
                }
                //写入文件
                QTextStream out(&file);
                file.open(QIODevice::WriteOnly);

                //char mp_xml_buf[1024*10];
                //对数组进行初始化
                //memset(mp_xml_buf,0,sizeof(mp_xml_buf));
                //memcpy(mp_xml_buf,buf+n_pos,length - n_pos);
                out << buf+n_pos << endl;
                qDebug()<<buf+n_pos;

                //解析xml文件
                parse(s_file_name);
            }else { //如果获取xml文件失败，则重新发送xml请求
                send_xml_request();
            }
        }

    }
    qDebug()<<"结束 seq_id"<<m_seq_id_list;
}

//播放视频回复
void Demo::play_reply(char * buf, int length)
{
    VodPlayResp vod_play_resp;
    memset( &vod_play_resp, 0, sizeof( VodPlayResp ) );
    memcpy( &vod_play_resp, buf ,length );
    int n_pos=0;
    n_pos = sizeof(vod_play_resp.header.msg_type);
    n_pos = sizeof(vod_play_resp.header.total_len) + n_pos;

    int n_count = m_seq_id_class_list.size();
    qDebug()<<"seq_id count:"<<n_count;
    qDebug()<<"开始 seq_id"<<m_seq_id_class_list;
    for(int i = 0; i < n_count; i++)
    {
        qDebug()<<(m_seq_id_class_list[i].split(","))[0].toInt()<<vod_play_resp.header.seq_id<<qFromBigEndian(vod_play_resp.header.seq_id);
        if( (m_seq_id_class_list[i].split(","))[0].toInt() == qFromBigEndian(vod_play_resp.header.seq_id) ){
            n_pos = sizeof(vod_play_resp.header);
            if( vod_play_resp.result == 1 ) {
                //list 排序，并比较seq_id 是否相等，相等，得到class的值：class=0 可见光  class=1 红外
                n_pos = n_pos + sizeof(vod_play_resp.result);

                qDebug()<<"n_class:"<<m_seq_id_class_list[i].split(",")[1].toInt();
                //获取视频数据并播放 url_str格式:IP:Port    192.168.0.123:5555  class=0,表示可见光
                receiver_play_message(vod_play_resp.url_buf, m_seq_id_class_list[i].split(",")[1].toInt());
            }
            m_seq_id_class_list.removeAt(i);
        } else { //重新发送播放请求
        //play_request();
        }
    }
}

void Demo::stop_play_reply(char * buf)
{
    VodStopPlayResp vod_stop_play_resp;
    memcpy(&vod_stop_play_resp,buf,sizeof(vod_stop_play_resp));

    int n_count = m_stop_seq_id_class_list.size();
    qDebug()<<"seq_id count:"<<n_count<<vod_stop_play_resp.result;
    qDebug()<<"开始 seq_id"<<m_stop_seq_id_class_list;
    for(int i = 0; i < n_count; i++)
    {
        qDebug()<<m_stop_seq_id_class_list[i].split(",")[0].toInt()<<m_stop_seq_id_class_list[i].split(",")[1].toInt()<<vod_stop_play_resp.header.seq_id<< qFromBigEndian(vod_stop_play_resp.header.seq_id);
        if( (m_stop_seq_id_class_list[i].split(","))[0].toInt() == qFromBigEndian(vod_stop_play_resp.header.seq_id) ){
            if(vod_stop_play_resp.result == 1 && (m_stop_seq_id_class_list[i].split(",")[1].toInt() == 0)) {
                mp_vi_tcp_client->disconnectFromHost();
                mp_vi_tcp_client->waitForDisconnected();
                mn_vi_connect_status = YF_Disconnected;
                m_vi_image.fill( QColor(200,200,200) );
            }else if(vod_stop_play_resp.result == 1 && (m_stop_seq_id_class_list[i].split(",")[1].toInt() == 1)){
                mp_ir_tcp_client->disconnectFromHost();
                mp_ir_tcp_client->waitForDisconnected();
                mn_ir_connect_status = YF_Disconnected;
                m_ir_image.fill( QColor(200,200,200) );
            }
            m_stop_seq_id_class_list.removeAt(i);
        }
   }
    update();
}

//对xml文件的解析
void Demo::parse(QString file_name)
{
    if(file_name.isEmpty()) return;

    //打开文件
    QFile file(file_name);
    if( !file.open( QFile::ReadOnly | QFile::Text ) ) {
        QMessageBox::information(NULL, QString("title"), QString("open error!"));  return;
    }

    QDomDocument document;
    QString error;
    int row = 0, column = 0;
    //setContent：设置内容
    if( !document.setContent(&file, false, &error, &row, &column) ) {
        QMessageBox::information(NULL, QString("title"), QString("parse file failed at line row and column")
                                 + QString::number(row, 10) + QString(",") + QString::number(column, 10));
        return;
    }

    if(document.isNull()) {
        QMessageBox::information(NULL, QString("title"), QString("document is null!"));
        return;
    }

    QDomElement root = document.documentElement();

    QString root_tag_name = root.tagName();
    if(root.hasAttribute("name"))  {
        QString name = root.attributeNode("name").value();
    }

    QDomElement person = root.firstChildElement();
    if( person.isNull() ) return;

    QString person_tag_name = person.tagName();
    QString id = person.attributeNode("id").value();

    QDomNodeList list = root.childNodes();
    int n_count = list.count();

    mp_standard_item_model->clear();

    QStringList s_list;
    for( int i=0; i<n_count; i++) {
        QDomNode dom_node = list.item(i);
        QDomElement element = dom_node.toElement();

        //获取id值，等价
        QString id_1 = element.attributeNode("id").value();
        QString id_2 = element.attribute("id");
        qDebug()<<id_1<<id_2;

        QDomNodeList child_list = element.childNodes();
        int child_count = child_list.count();

        DeviceInfo device_info;
        for(int j=0; j<child_count;j++)
        {
            QDomNode child_dom_node = child_list.item(j);
            QDomElement child_element = child_dom_node.toElement();
            QString child_tag_name = child_element.tagName();
            QString child_tag_value = child_element.text();

            qDebug()<<child_tag_name<<child_tag_value;

            if( child_tag_name == "class" ) {
                device_info.n_class = child_tag_value.toInt();
            } else if( child_tag_name == "type" ) {
                device_info.s_type = child_tag_value;
            } else if( child_tag_name == "sn" ) {
                device_info.s_sn = child_tag_value;
            }else if( child_tag_name == "ip" ) {
                device_info.s_ip = child_tag_value;
            }else if( child_tag_name == "port" ) {
                device_info.n_port = child_tag_value.toInt();
            }else if( child_tag_name == "mac" ){
                device_info.s_mac = child_tag_value;
            }
        }

        QString str_item = (device_info.n_class==0?"vi,":"ir,") + device_info.s_sn;
        QStandardItem *item = new QStandardItem(str_item);
        /*if(i % 2 == 1)  {
            QLinearGradient linearGrad(QPointF(0, 0), QPointF(200, 200));
            linearGrad.setColorAt(0, Qt::darkGreen);
            linearGrad.setColorAt(1, Qt::yellow);
            QBrush brush(linearGrad);
            item->setBackground(brush);
        }*/
        item->setToolTip( str_item );
        mp_standard_item_model->appendRow(item);
    }

    int nCount = s_list.size();
    qDebug()<<nCount;
    for(int i = 0; i < nCount; i++)
    {

    }

}

void Demo::paintEvent(QPaintEvent * event)
{
    QPainter draw;
    draw.begin( this );

    int n_start_time = QTime::currentTime().msecsSinceStartOfDay();
     m_mutex_ir.lock();

    //红外显示//n_height*4/5+SPACE,SPACE
    if( m_ir_image.isNull() ) {
        //draw.fillRect( 165, 50, 640, 480, QBrush( QColor( 200, 200, 200 )));
        draw.fillRect( WIDTH_LIST_VIEW_DEVICE+SPACE, ui->id_main_tool_bar->height()+SPACE, ui->id_frame_ir->width(), ui->id_frame_ir->height(), QBrush( QColor( 200, 200, 200 )));
    } else {
        //QRect rect = ui->id_frame_ir->rect();
        //draw.drawImage( 165, 50, m_ir_image, 165, 50, ui->id_frame_ir->width(), ui->id_frame_ir->height() );
        draw.drawImage(QRect(WIDTH_LIST_VIEW_DEVICE+SPACE, ui->id_main_tool_bar->height()+SPACE, ui->id_frame_ir->width(), ui->id_frame_ir->height()), m_ir_image);
        //qDebug()<<"frame:"<<ui->id_frame_ir->width()<<ui->id_frame_ir->height()<<m_ir_image.width()<<m_ir_image.height();
    }
    m_mutex_ir.unlock();

     m_mutex_vi.lock();

    //可见光显示
    if( m_vi_image.isNull() ) {
        draw.fillRect( WIDTH_LIST_VIEW_DEVICE+SPACE+ui->id_frame_ir->width(), ui->id_main_tool_bar->height()+SPACE, ui->id_frame_vi->width(), ui->id_frame_vi->height(), QBrush( QColor( 200, 200, 200 )));
    } else {
        draw.drawImage(QRect(WIDTH_LIST_VIEW_DEVICE+SPACE+ui->id_frame_ir->width(), ui->id_main_tool_bar->height()+SPACE, ui->id_frame_vi->width(), ui->id_frame_vi->height()), m_vi_image);
        //draw.drawImage(QRect(WIDTH_LIST_VIEW_DEVICE+SPACE+ui->id_textedit_device->width(), ui->id_main_tool_bar->height()+SPACE + ui->id_frame_ir->height(), ui->id_frame_ana->width(), ui->id_frame_ana->height()), m_vi_image);
    }

    m_mutex_vi.unlock();

    //qDebug()<< "paintEvent:"  << QTime::currentTime().msecsSinceStartOfDay() - n_start_time;
    draw.end();
}

//双击listview
void Demo::on_id_listview_device_doubleClicked(const QModelIndex &index)
{
    QString str_item = ( ( QStandardItemModel *) ui->id_listview_device->model() )->item( index.row() )->text();
    QString str_device_sn = str_item.split(",")[1];
    QString str_device_class = str_item.split(",")[0];

    int n_class = str_device_class=="vi"?0:1;

    int n_connect_status =  n_class==0 ? mn_vi_connect_status : mn_ir_connect_status;

    if( n_connect_status == YF_Disconnected) {
        if ( n_class == 0 ) {
            mn_vi_connect_status = YF_Connecting;
        } else {
            mn_ir_connect_status = YF_Connecting;
        }

        connect_play( str_device_sn, n_class );
    } else if ( n_connect_status  == YF_Connected ) {
        if ( n_class == 0 ) {
            mn_vi_connect_status = YF_Disconnecting;
        } else {
            mn_ir_connect_status = YF_Disconnecting;
        }

        disconnect_play( str_device_sn, n_class );
    } else if (  n_connect_status  == YF_Connecting ) {
        qDebug() << str_item << "正在连接...";
    } else {
        qDebug() << str_item << "正在断开...";
    }
}

void Demo::update_show()
{
    update();
}

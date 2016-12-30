#include "StandbyAdvertWidget.h"
#include "ui_StandbyAdvertWidget.h"
#include <QPainter>

StandbyAdvertWidget::StandbyAdvertWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StandbyAdvertWidget)
  , m_pTimer( NULL )
  , m_nTimerId( -1 )
  , m_nCurFilePos( -1 )
{
    ui->setupUi(this);

    initCtrl();
}

StandbyAdvertWidget::~StandbyAdvertWidget()
{
    stopTimer();

    delete ui;
}

void StandbyAdvertWidget::setFileList( QList<QString> listFilePath )
{
    stopTimer();
    QList<QString>::iterator it = listFilePath.begin();
    for( int i=1; it != listFilePath.end(); ++it,++i ) {
        m_mapFilePath.insert( i, *it );
    }

    m_image = QImage( m_mapFilePath.begin().value() );
    m_nCurFilePos = 1;
    startTimer();
}

void StandbyAdvertWidget::setFileList( QMap<int, QString> mapFilePath )
{
    stopTimer();
    m_mapFilePath = mapFilePath;
    m_image = QImage( m_mapFilePath.begin().value() );
    m_nCurFilePos = 1;

//    QList<QString>::iterator it = listVideoPath.begin();
//    for( int i=1; it != listVideoPath.end(); ++it,++i ) {
//        m_mapVideoPath.insert( i, *it );
//    }

//    m_nCurVideoPos = 1;

//    QMediaPlayer *player = new QMediaPlayer(this);

//    QVideoWidget *videoWidget = new QVideoWidget(this);

//    player->setVideoOutput( videoWidget );
//    player->setMedia( QUrl(/*"http://example.com/movie.mp4")*/ "G://xunlei/lastDreaM.rmvb" ) );

//    videoWidget->show()

    startTimer();
    update();
}

bool StandbyAdvertWidget::getFileList(QList<QString> &listFilePath)
{
    Q_UNUSED( listFilePath )
    return true;
}

void StandbyAdvertWidget::paintEvent( QPaintEvent *event )
{
    Q_UNUSED( event )

    QPainter draw( this );

    draw.drawImage( QRect(0,0, width(), height()), m_image );

    draw.end();
}

void StandbyAdvertWidget::initCtrl()
{
    m_pTimer = new QTimer( this );
    connect( m_pTimer, SIGNAL(timeout()), this, SLOT( slot_updateImage() ) );

//    QString str1 = ":/customer/customer1.jpg";
//    QString str2 = ":/customer/customer2.jpg";
//    QString str3 = ":/customer/customer3.jpg";
    QString str1 = ":/customer1.jpg";
    QString str2 = ":/customer2.jpg";
    QString str3 = ":/customer3.jpg";


    QMap<int, QString> mapFilePath;
    mapFilePath.insert( 1, str1 );
    mapFilePath.insert( 2, str2 );
    mapFilePath.insert( 3, str3 );

    setFileList( mapFilePath );
}

void StandbyAdvertWidget::startTimer()
{
    if( m_nTimerId < 0 ) {
        m_pTimer->start( 5000 );
        m_nTimerId = m_pTimer->timerId();
    }
}

void StandbyAdvertWidget::stopTimer()
{
    if( m_nTimerId> 0 ) {
        m_pTimer->stop();
        m_nTimerId = -1;
    }
}

void StandbyAdvertWidget::slot_updateImage()
{
    ++m_nCurFilePos;
    if( m_nCurFilePos > m_mapFilePath.size() )
    {
        m_nCurFilePos = 1;
    }
    m_image = QImage( m_mapFilePath[m_nCurFilePos] );
    update();
}

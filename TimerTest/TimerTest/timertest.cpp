#include "timertest.h"
#include "ui_timertest.h"
#include <QTimer>

TimerTest::TimerTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TimerTest)
{
    ui->setupUi(this);
    init_connect();
}

TimerTest::~TimerTest()
{
    delete ui;
}

void TimerTest::init_connect()
{
    connect( ui->pushButton, SIGNAL( clicked() ), this, SLOT( slot_btn_1() ) );
    connect( ui->id_btn_sender, SIGNAL( clicked() ), this, SLOT( slot_btn_sender() ) );
}

void TimerTest::slot_btn_1()
{
    ui->pushButton->setEnabled( false );
    m_list_time.push_back( btn1 );

    QTimer *p_timer = new QTimer( this );
    p_timer->start( 2000 );

    connect( p_timer, SIGNAL(timeout()), this, SLOT( slot_btn_effective() ) );

}

void TimerTest::slot_btn_sender()
{
    ui->id_btn_sender->setEnabled( false );
    m_list_time.push_back( btn2 );

    QTimer *p_timer = new QTimer( this );
    p_timer->start( 2000 );
    connect( p_timer, SIGNAL(timeout()), this, SLOT( slot_btn_effective() ) );
}

void TimerTest::slot_btn_effective()
{
    QObject *p_object = QObject::sender();
    QTimer *p_timer = (QTimer*)p_object;

    QList<int>::iterator it_int = m_list_time.begin();
    if ( it_int !=m_list_time.end() )
    {
        //on_resp( *it, true  );
        if(  *it_int == btn1 ) {
            m_list_time.pop_front();
            ui->pushButton->setEnabled( true );

            p_timer->stop();
            delete p_timer;
            p_timer = NULL;
        } else if( *it_int== btn2 ) {
            m_list_time.pop_front();
            ui->id_btn_sender->setEnabled( true );

            p_timer->stop();
            delete p_timer;
            p_timer = NULL;
        }
    }
}


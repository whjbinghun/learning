#include "backplay.h"
#include "ui_backplay.h"
#include <QPainter>

#include <QDebug>

#define WIDTH 150
#define HEIGHT 150
#define PLAY_TIME_SECTION 12
#define PRE_DAY_BTN_WIDTH 30
#define SPACE 20

BackPlay::BackPlay(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BackPlay)
    ,mp_calendar( NULL )
    ,mp_grid_layout( NULL )
{
    ui->setupUi(this);
    createPreviewGroupBox();
}

BackPlay::~BackPlay()
{
    delete ui;
}


//! [9]
void BackPlay::createPreviewGroupBox()
{
    //previewGroupBox = new QGroupBox(tr("Preview"));

    mp_calendar = new QCalendarWidget;//日历窗口
    mp_calendar->setMinimumDate(QDate(1900, 1, 1));
    mp_calendar->setMaximumDate(QDate(3000, 1, 1));
    mp_calendar->setGridVisible(true);//设置网格

    //connect(mp_calendar, SIGNAL(currentPageChanged(int,int)),
    //        this, SLOT(reformatCalendarPage()));

    mp_grid_layout = new QGridLayout;//网格布局
    mp_grid_layout->addWidget(mp_calendar, 0, 0, Qt::AlignCenter);
    ui->id_backplay_group_box->setLayout(mp_grid_layout);
    connect(mp_calendar, SIGNAL(selectionChanged()),
            this, SLOT(selectedDateChanged()));
}
//! [9]

void BackPlay::resizeEvent(QResizeEvent *event)
{
    //ui->id_backplay_group_box->resize( WIDTH, HEIGHT );
    ui->id_time_widget->setStyleSheet( "border:1px solid black" );
}

void BackPlay::paintEvent( QPaintEvent *event )
{
    QPainter draw;
    draw.begin( this );
    int n_x = ui->id_time_widget->x();
    int n_y =ui->id_time_widget->y();
    int width = ui->id_time_widget->width()-PRE_DAY_BTN_WIDTH*2-SPACE*2;
    int n_one_section_width = width/PLAY_TIME_SECTION;
    int n_init_width = n_x+PRE_DAY_BTN_WIDTH+SPACE;
    for( int i=0; i<=PLAY_TIME_SECTION; i++ ) {
        draw.drawLine( QLineF( n_init_width+n_one_section_width*i, n_y+SPACE*2, n_init_width+n_one_section_width*i, n_y+SPACE*2+10 )  );
        draw.drawText( QRectF( n_init_width+n_one_section_width*i, n_y+SPACE*3/2, 10, 10 ), QString::number( i*2 )+":00" );
    }



    draw.end();
}


//! [2]
void BackPlay::selectedDateChanged()
{
    mp_calendar->selectedDate();
    qDebug()<<"BackPlay::selectedDateChanged"<<mp_calendar->selectedDate();
}
//! [2]

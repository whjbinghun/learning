#include "wndplayrate.h"
#include "ui_wndplayrate.h"
#include <QPainter>
#include <QGraphicsPolygonItem>
#include <QFont>

#define SPACE 10
#define BUTTON_HEIGHT 10
#define LOW_HEIGHT 8
#define HIGH_HEIGHT 3
#define TEXT_WIDTH 25
#define TEXT_HEIGHT 13
#define SLIDER_WIDTH 10


WndPlayrate::WndPlayrate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WndPlayrate)
    ,mn_slider_position( 0 )
{
    ui->setupUi(this);
    cal_mark_pos();
}

WndPlayrate::~WndPlayrate()
{
    delete ui;
}

void WndPlayrate::init_graphical()
{
    m_vector_pt.clear();
    m_vector_pt.push_back( QPoint( SPACE, LOW_HEIGHT ) );
    m_vector_pt.push_back( QPoint( width()-SPACE, HIGH_HEIGHT ) );
    m_vector_pt.push_back( QPoint( width()-SPACE, BUTTON_HEIGHT ) );
    m_vector_pt.push_back( QPoint( SPACE, BUTTON_HEIGHT ) );
}

void WndPlayrate::init_pt_pos()
{
    m_vector_rate.clear();
    float f_eq_pos = (width()-20)/6.0;
    insert_mark( 0, 0.125, "1/8" );
    insert_mark( f_eq_pos, 0.25, "1/4" );
    insert_mark( f_eq_pos*2, 0.5, "1/2" );
    insert_mark( f_eq_pos*3, 1, "1x" );
    insert_mark( f_eq_pos*4, 2, "2" );
    insert_mark( f_eq_pos*5, 4, "4" );
    insert_mark( f_eq_pos*6, 8, "8" );
}

void WndPlayrate::insert_mark( float f_pos, float f_value, QString str_text )
{
    RateInfo rate_info;
    rate_info.f_position = f_pos;
    rate_info.f_value = f_value;
    rate_info.str_text_name = str_text;
    m_vector_rate.push_back( rate_info );
}

void WndPlayrate::cal_mark_pos()
{
    init_graphical();
    init_pt_pos();
}

void WndPlayrate::paintEvent( QPaintEvent *p_event )
{
    QPainter draw( this );

    QFont ft;
    ft.setPointSize( 10 );
    draw.setFont( ft );
    //画梯形
    QPolygon poly;
    poly = QPolygon( m_vector_pt );
    draw.drawConvexPolygon( poly );

    QVector<RateInfo>::iterator it_rate = m_vector_rate.begin();
    int n_line_height = 2;
    for( ; it_rate!=m_vector_rate.end(); it_rate++ ) {
        //画线
        draw.drawLine( QPoint( it_rate->f_position+SPACE, BUTTON_HEIGHT ), QPoint( it_rate->f_position+SPACE, BUTTON_HEIGHT+n_line_height ) );
        //画文本
        draw.drawText( QRect( it_rate->f_position+SPACE-TEXT_WIDTH/2.0, BUTTON_HEIGHT+n_line_height, TEXT_WIDTH, TEXT_HEIGHT ), Qt::AlignCenter, it_rate->str_text_name );
    }

    //画滑动块   滑动块的高度是24，宽度为10
    draw.fillRect( QRect(m_vector_rate[mn_slider_position].f_position+SLIDER_WIDTH/2, HIGH_HEIGHT, SLIDER_WIDTH, BUTTON_HEIGHT+n_line_height), QColor( 219, 218, 218) );
    draw.setPen( QColor( 107, 107, 107) );
    draw.drawRect( QRect(m_vector_rate[mn_slider_position].f_position+SLIDER_WIDTH/2, HIGH_HEIGHT, SLIDER_WIDTH, BUTTON_HEIGHT+n_line_height) );

    draw.end();
}

void WndPlayrate::resizeEvent( QResizeEvent *p_event )
{
    cal_mark_pos();
}

void WndPlayrate::mousePressEvent( QMouseEvent *p_event )
{
    if(  p_event->button() == Qt::LeftButton ) {
        int n_x = p_event->x();
        int n_eq_width = (int)m_vector_rate[1].f_position;
        if( n_x<10 ) {
            mn_slider_position = 0;
            emit sig_rate_num( m_vector_rate[mn_slider_position].f_value );
            update();
            return;
        } else if( n_x>width()-10 ) {
            mn_slider_position = m_vector_rate.size()-1;
            emit sig_rate_num( m_vector_rate[mn_slider_position].f_value );
            update();
            return;
        }
        QVector<RateInfo>::Iterator it_rate = m_vector_rate.begin();
        for( int i=0; it_rate!=m_vector_rate.end(); i++, it_rate++ ) {
            if( abs( n_x-10-it_rate->f_position )<=n_eq_width/2  ) {
                mn_slider_position = i;
                emit sig_rate_num( m_vector_rate[mn_slider_position].f_value );
                update();
                break;
            }
        }
    }
}

#include "switchwinnum.h"
#include "ui_switchwinnum.h"
#include <QStandardItem>
#include <QTextCodec>
#include <QTreeWidgetItem>
#include <QGraphicsOpacityEffect>

#include <QDebug>

#define GROUP_BOX_WIDTH 240
#define SPACE 20
#define LAB_WIDTH 60
#define LAB_HEIGHT 40

SwitchWinNum::SwitchWinNum(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SwitchWinNum)
    ,mn_widget_show_num( 2 )  //默认为2个窗口
    ,mb_mouse_press( false )
{
    ui->setupUi(this);
    init_group_list();
    init_list_view();
    init_connect();
    show_frame();

    ui->id_lab_move->setWindowFlags( ui->id_lab_move->windowFlags() | Qt::WindowStaysOnTopHint );
    ui->id_lab_move->hide();

    ui->id_show_name_1->show();
    ui->id_show_name_2->show();
    ui->id_show_name_3->show();
    ui->id_show_name_4->show();
    ui->id_show_name_5->show();
    ui->id_show_name_6->show();
}

SwitchWinNum::~SwitchWinNum()
{
    delete ui;
}

void SwitchWinNum::resizeEvent( QResizeEvent *event )
{
    //setStyleSheet( "border:1px solid black" );
    move_widget();
    ui->id_frame_1->setStyleSheet( "border:1px solid black" );
    ui->id_frame_2->setStyleSheet( "border:1px solid black" );
    ui->id_frame_3->setStyleSheet( "border:1px solid black" );
    ui->id_frame_4->setStyleSheet( "border:1px solid black" );
    ui->id_frame_5->setStyleSheet( "border:1px solid black" );
    ui->id_frame_6->setStyleSheet( "border:1px solid black" );


}

void SwitchWinNum::init_group_list()
{
    ui->id_listView->setMouseTracking(1);
    ui->id_listView->setContextMenuPolicy( Qt::CustomContextMenu );

    QTextCodec::setCodecForLocale( QTextCodec::codecForName( "GBK" ) );
    QStandardItemModel *model = new QStandardItemModel( 4, 1 );
    model->setHeaderData( 0, Qt::Horizontal, tr( "Service" ) );
    QStandardItem *item1 = new QStandardItem( "avahi-daemon" );
    QStandardItem *item2 = new QStandardItem( "bluetooth" );
    QStandardItem *item3 = new QStandardItem( "crond" );
    QStandardItem *item4 = new QStandardItem( "cups" );
    model->setItem( 0, 0, item1 );
    model->setItem( 1, 0, item2 );
    model->setItem( 2, 0, item3 );
    model->setItem( 3, 0, item4 );
    QStandardItem *item5 = new QStandardItem( "fifth" );
    item4->appendRow( item5 );//把item5结点加入到item4中

    QModelIndex model_index;
    //向item1中加入4个结点
    for ( int i = 0; i < 4; ++i ) {
        model_index = model->index( 0, 0, model_index );
        model->insertRows( 0, 1, model_index );
        model->insertColumns( 0, 1, model_index );
        QModelIndex index = model->index( 0, 0, model_index );
        model->setData( index, i );
    }
    ui->id_listView->setModel( model );
}

void SwitchWinNum::init_list_view()
{
    ui->id_listView->setAcceptDrops(true);
    ui->id_listView->set_wnd( ui->id_lab_move, this );
}

//pt表示在当前整个窗口的位置
void SwitchWinNum::mouse_release( const QPointF &pt, int nx, int ny, QString str_group_name )
{  
    if ( !ui->id_listView->get_press() ) return;
    if( mn_widget_show_num == 2 ) {
        if( pt.x()>=ui->id_frame_1->x() && pt.x()<ui->id_frame_1->x()+ui->id_frame_1->width() \
                && pt.y()>=ui->id_frame_1->y() && pt.y()<ui->id_frame_1->y()+ui->id_frame_1->height() ) {
            ui->id_show_name_1->setText( str_group_name );
            change_group_name( 1, str_group_name );
        } else if( pt.x()>=ui->id_frame_2->x() && pt.x()<ui->id_frame_2->x()+ui->id_frame_2->width() \
                && pt.y()>=ui->id_frame_2->y() && pt.y()<ui->id_frame_2->y()+ui->id_frame_2->height() ) {
            ui->id_show_name_2->setText( str_group_name );
            change_group_name( 2, str_group_name );
        }
    } else if( mn_widget_show_num == 6 ) {
        if( pt.x()>=ui->id_frame_1->x() && pt.x()<ui->id_frame_1->x()+ui->id_frame_1->width() \
                && pt.y()>=ui->id_frame_1->y() && pt.y()<ui->id_frame_1->y()+ui->id_frame_1->height() ) {
            ui->id_show_name_1->setText( str_group_name );
            change_group_name( 1, str_group_name );
        } else if( pt.x()>=ui->id_frame_2->x() && pt.x()<ui->id_frame_2->x()+ui->id_frame_2->width() \
                && pt.y()>=ui->id_frame_2->y() && pt.y()<ui->id_frame_2->y()+ui->id_frame_2->height() ) {
            ui->id_show_name_2->setText( str_group_name );
            change_group_name( 2, str_group_name );
        } else  if( pt.x()>=ui->id_frame_3->x() && pt.x()<ui->id_frame_3->x()+ui->id_frame_3->width() \
                    && pt.y()>=ui->id_frame_3->y() && pt.y()<ui->id_frame_3->y()+ui->id_frame_3->height() ) {
            ui->id_show_name_3->setText( str_group_name );
            change_group_name( 3, str_group_name );
        } else if( pt.x()>=ui->id_frame_4->x() && pt.x()<ui->id_frame_4->x()+ui->id_frame_4->width() \
                  && pt.y()>=ui->id_frame_4->y() && pt.y()<ui->id_frame_4->y()+ui->id_frame_4->height() ) {
            ui->id_show_name_4->setText( str_group_name );
            change_group_name( 4, str_group_name );
        } else if( pt.x()>=ui->id_frame_5->x() && pt.x()<ui->id_frame_5->x()+ui->id_frame_5->width() \
                && pt.y()>=ui->id_frame_5->y() && pt.y()<ui->id_frame_5->y()+ui->id_frame_5->height() ) {
            ui->id_show_name_5->setText( str_group_name );
            change_group_name( 5, str_group_name );
        } else if( pt.x()>=ui->id_frame_6->x() && pt.x()<ui->id_frame_6->x()+ui->id_frame_6->width() \
              && pt.y()>=ui->id_frame_6->y() && pt.y()<ui->id_frame_6->y()+ui->id_frame_6->height() ) {
            ui->id_show_name_6->setText( str_group_name );
            change_group_name( 6, str_group_name );
      }
    }
    ui->id_listView->set_press( false );
    ui->id_lab_move->hide();
    update();

}

void SwitchWinNum::move_widget()
{
    ui->centralWidget->move( 0, 0 );
    ui->centralWidget->resize( width(), height() );
    int n_width = ui->centralWidget->width()-GROUP_BOX_WIDTH-3*SPACE;
    int n_height = ui->centralWidget->height()-2*SPACE;
    ui->id_lab_group_list->move( SPACE, SPACE );
    ui->id_lab_group_list->resize( GROUP_BOX_WIDTH, SPACE*2 );
    ui->id_listView->move( SPACE, SPACE*3 );
    ui->id_listView->resize( GROUP_BOX_WIDTH, (ui->centralWidget->height()-3*SPACE)/2 );
    ui->id_lab_widget_num->move( SPACE, SPACE*4+ui->id_listView->height() );
    ui->id_lab_widget_num->resize( GROUP_BOX_WIDTH, SPACE*2 );
    ui->id_btn_two_window->move( SPACE*2, SPACE*6+ui->id_listView->height() );
    ui->id_btn_six_window->move( GROUP_BOX_WIDTH-ui->id_btn_six_window->width(), SPACE*6+ui->id_listView->height() );
    QRect rct( SPACE*2+GROUP_BOX_WIDTH, SPACE, n_width, n_height );
    int rct_x = rct.x();
    int rct_y = rct.y();
    int rct_width = rct.width();
    int rct_height = rct.height();

    if( mn_widget_show_num == 2 ) {
        ui->id_frame_1->move( rct_x, rct_y );
        ui->id_frame_1->resize( rct_width, rct_height/2 );
        ui->id_show_name_1->move( (rct_width-LAB_WIDTH)/2, (rct_height/2-LAB_HEIGHT)/2 );
        ui->id_show_name_1->resize( LAB_WIDTH, LAB_HEIGHT );
        ui->id_frame_2->move( rct_x, rct_y+rct_height/2 );
        ui->id_frame_2->resize( rct_width, rct_height/2 );
        ui->id_show_name_2->move( (rct_width-LAB_WIDTH)/2, (rct_height/2-LAB_HEIGHT)/2 );
        ui->id_show_name_2->resize( LAB_WIDTH, LAB_HEIGHT );
        show_frame();
    } else if( mn_widget_show_num == 6 ) {
        ui->id_frame_1->move( rct_x, rct_y );
        ui->id_frame_1->resize( rct_width/2, rct_height/3 );
        ui->id_show_name_1->move( (rct_width/2-LAB_WIDTH)/2, (rct_height/3-LAB_HEIGHT)/2 );
        ui->id_show_name_1->resize( LAB_WIDTH, LAB_HEIGHT );
        ui->id_frame_2->move( rct_x+rct_width/2, rct_y );
        ui->id_frame_2->resize( rct_width/2, rct_height/3 );
        ui->id_show_name_2->move( (rct_width/2-LAB_WIDTH)/2, (rct_height/3-LAB_HEIGHT)/2 );
        ui->id_show_name_2->resize( LAB_WIDTH, LAB_HEIGHT );

        ui->id_frame_3->move( rct_x, rct_y+rct_height/3 );
        ui->id_frame_3->resize( rct_width/2, rct_height/3 );
        ui->id_show_name_3->move( (rct_width/2-LAB_WIDTH)/2, (rct_height/3-LAB_HEIGHT)/2 );
        ui->id_show_name_3->resize( LAB_WIDTH, LAB_HEIGHT );
        ui->id_frame_4->move( rct_x+rct_width/2, rct_y+rct_height/3 );
        ui->id_frame_4->resize( rct_width/2, rct_height/3 );
        ui->id_show_name_4->move( (rct_width/2-LAB_WIDTH)/2, (rct_height/3-LAB_HEIGHT)/2 );
        ui->id_show_name_4->resize( LAB_WIDTH, LAB_HEIGHT );

        ui->id_frame_5->move( rct_x, rct_y+rct_height*2/3 );
        ui->id_frame_5->resize( rct_width/2, rct_height/3 );
        ui->id_show_name_5->move( (rct_width/2-LAB_WIDTH)/2, (rct_height/3-LAB_HEIGHT)/2 );
        ui->id_show_name_5->resize( LAB_WIDTH, LAB_HEIGHT );
        ui->id_frame_6->move( rct_x+rct_width/2, rct_y+rct_height*2/3 );
        ui->id_frame_6->resize( rct_width/2, rct_height/3 );
        ui->id_show_name_6->move( (rct_width/2-LAB_WIDTH)/2, (rct_height/3-LAB_HEIGHT)/2 );
        ui->id_show_name_6->resize( LAB_WIDTH, LAB_HEIGHT );

        show_frame();
    }


}

void SwitchWinNum::init_connect()
{
    connect( ui->id_btn_two_window, SIGNAL( clicked() ), this, SLOT( slot_show_two_widget() ) );
    connect( ui->id_btn_six_window, SIGNAL( clicked() ), this, SLOT( slot_show_six_widget() ) );
}

void SwitchWinNum::show_frame()
{
    if( mn_widget_show_num == 2 ) {
        ui->id_frame_1->show();
        ui->id_frame_2->show();
        ui->id_frame_3->hide();
        ui->id_frame_4->hide();
        ui->id_frame_5->hide();
        ui->id_frame_6->hide();
        ui->id_show_name_1->show();
        ui->id_show_name_2->show();
        ui->id_show_name_3->hide();
        ui->id_show_name_4->hide();
        ui->id_show_name_5->hide();
        ui->id_show_name_6->hide();
    } else if( mn_widget_show_num == 6 ) {
        ui->id_frame_1->show();
        ui->id_frame_2->show();
        ui->id_frame_3->show();
        ui->id_frame_4->show();
        ui->id_frame_5->show();
        ui->id_frame_6->show();
        ui->id_show_name_1->show();
        ui->id_show_name_2->show();
        ui->id_show_name_3->show();
        ui->id_show_name_4->show();
        ui->id_show_name_5->show();
        ui->id_show_name_6->show();
    }
}

void SwitchWinNum::slot_show_two_widget()
{
    mn_widget_show_num = 2;
    move_widget();
}

void SwitchWinNum::slot_show_six_widget()
{
    mn_widget_show_num = 6;
    move_widget();
}

//窗口属性
void SwitchWinNum::widget_attribute()
{
    ui->id_frame_1->setStyleSheet( "border:1px solid black" );
    ui->id_frame_2->setStyleSheet( "border:1px solid black" );
    ui->id_frame_3->setStyleSheet( "border:1px solid black" );
    ui->id_frame_4->setStyleSheet( "border:1px solid black" );
    ui->id_frame_5->setStyleSheet( "border:1px solid black" );
    ui->id_frame_6->setStyleSheet( "border:1px solid black" );
}

void SwitchWinNum::add_group_name( int n_num, QString str_group_name )
{
    //str_group_name不能相同，如果相同？？？
    m_map_group_name.insert( n_num, str_group_name );
}

void SwitchWinNum::del_group_name( int n_num )
{
    m_map_group_name.remove( n_num );
}

void SwitchWinNum::change_group_name( int n_num, QString str_group_name )
{
    QMap<int,QString>::iterator it;
    it=m_map_group_name.begin();
    for( ; it!=m_map_group_name.end(); it++ ) {
        qDebug()<<it.key()<<it.value()<<n_num<<str_group_name;
        if( it.key() != n_num && it.value()==str_group_name ) {
            //组lab隐藏
            switch( it.key() ) {
            case 1:
                ui->id_show_name_1->setText( "显示组号1" );
                break;
            case 2:
                ui->id_show_name_2->setText( "显示组号2" );
                break;
            case 3:
                ui->id_show_name_3->setText( "显示组号3" );
                break;
            case 4:
                ui->id_show_name_4->setText( "显示组号4" );
                break;
            case 5:
                ui->id_show_name_5->setText( "显示组号5" );
                break;
            case 6:
                ui->id_show_name_6->setText( "显示组号6" );
                break;
            }
            m_map_group_name.remove( it.key() );
            break;
        }
    }
    add_group_name( n_num, str_group_name );
}

void SwitchWinNum::init_lab_move() //配置界面初始化
{
    QGraphicsOpacityEffect *effect=new QGraphicsOpacityEffect( this );
    effect->setOpacity( 0.8 );
    ui->id_lab_move->setGraphicsEffect(effect);
    ui->id_lab_move->hide();
}


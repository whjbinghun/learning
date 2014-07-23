#include "titlebar.h"
#include "vodplayinterface.h"
#include <QBitmap>

#include <QDebug>

#define TITLE_H 50
#define VALUE_DIS 10
//标题栏中按钮宽度  根据图片大小来算出一个按钮的宽度 和高度
#define BUTTON_WIDTH 47
#define BUTTON_HEIGHT 28
#define TITLE_SPACE 10


TitleBar::TitleBar(QWidget *parent) :
    QWidget(parent)
{
    mb_left_button_press = false;
    CreateWidget();

    //mp_btn_close->move( parent->width()-50, 0 );
    //发送主窗口缩小的信号
    connect( mp_btn_min, SIGNAL( clicked() ), parent, SIGNAL( min_window() ) );
    //主窗口最大化的信号
    connect( mp_btn_max, SIGNAL( clicked() ), parent, SIGNAL( max_window() ) );
    //发送关闭主窗口的信号
    connect( mp_btn_close, SIGNAL( clicked() ), parent, SIGNAL( close_window() ) );
    //点击配置按钮，显示配置窗口
    connect( mp_btn_monitor_interface, SIGNAL( clicked() ), parent, SIGNAL( sig_display_monitor_interface() ) );
    connect( mp_btn_config_interface, SIGNAL( clicked() ), parent, SIGNAL( sig_display_config_interface() ) );
    connect( mp_btn_analyse_interface, SIGNAL( clicked() ), parent, SIGNAL( sig_display_analyse_interface() ) );
    connect( mp_btn_backplay_interface, SIGNAL( clicked() ), parent, SIGNAL( sig_display_back_interface() ) );

    //setMouseTracking( true );

}


//创建子部件
void TitleBar::CreateWidget()
{
    //设置字号
    QFont ft;
    ft.setPointSize( 16 );
    //ui.label_4->setFont(ft);

    //图像标签--logo
    mp_label_icon = new QLabel( this );
    QPixmap objPixmap( "");//../image/360AboutLogo.png");
    QBitmap mask = objPixmap.createMaskFromColor( QColor( 255, 255, 255 ), Qt::MaskInColor );
    objPixmap.setMask( mask );
    mp_label_icon->setPixmap( objPixmap.scaled( TITLE_H, TITLE_H ) );
    mp_label_icon->setMinimumSize( mp_label_icon->sizeHint() );

    //setFrameStyle 去除label边框，但实际没有发挥作用
    //mp_label_icon->setFrameStyle(QFrame::NoFrame);


    //文本标签--标题
    mp_label_title = new QLabel( this );
    mp_label_title->setText( QString( "美盛监控客户端" ) );
    //mp_label_title->setMinimumSize( mp_label_title->sizeHint() );
    mp_label_title->setMinimumHeight( TITLE_H );
    mp_label_title->setMinimumWidth( 23 );
    //设置标签中的字体为白色
    mp_label_title->setStyleSheet( "color: white" );//setstylesheet( "font-color: white" );
    mp_label_title->setFont( ft );


    //文本标签--样式版本
    /*mp_label_version = new QLabel( this );
    mp_label_version->setText( QString( "Use Class Style" ) );
    //设置鼠标形状
    mp_label_version->setCursor( Qt::PointingHandCursor );*/
    //按钮--最小化
    mp_btn_min = new QToolButton( this );
    SetBtnIcon( mp_btn_min, eBtnStateDefault, true );
    //按钮--最大化
    mp_btn_max = new QToolButton( this );
    SetBtnIcon( mp_btn_max, eBtnStateDefault, true );
    //按钮--关闭
    mp_btn_close = new QToolButton( this );
    SetBtnIcon( mp_btn_close, eBtnStateDefault, true );

    mp_btn_monitor_interface = new QPushButton( this );
    mp_btn_monitor_interface->setText( tr( "返回主窗口" ) );
    //mp_btn_monitor_interface->show();
    mp_btn_monitor_interface->hide();
    mp_btn_monitor_interface->setGeometry( 100, 100, BUTTON_WIDTH*2, BUTTON_HEIGHT );

    //mp_btn_monitor_interface->setStyleSheet("color:red");

    mp_btn_config_interface = new QPushButton( this );
    mp_btn_config_interface->setText( tr( "配置" ) );
    mp_btn_config_interface->show();
    mp_btn_config_interface->setGeometry( 100, 100, BUTTON_WIDTH, BUTTON_HEIGHT );

    mp_btn_analyse_interface = new QPushButton( this );
    mp_btn_analyse_interface->setText( tr( "分析" ) );
    mp_btn_analyse_interface->show();
    mp_btn_analyse_interface->setGeometry( 100, 100, BUTTON_WIDTH, BUTTON_HEIGHT );

    mp_btn_backplay_interface = new QPushButton( this );
    mp_btn_backplay_interface->setText( tr( "回放" ) );
    mp_btn_backplay_interface->show();
    mp_btn_backplay_interface->setGeometry( 100, 100, BUTTON_WIDTH, BUTTON_HEIGHT );


    //获得子部件
    const QObjectList &objList = children();
    qDebug()<<"子部件"<<objList[0];
    for(int nIndex=0; nIndex<objList.count(); ++nIndex)
    {
        //设置子部件的MouseTracking属性
        (  (QWidget*)(objList.at(nIndex)))->setMouseTracking(true);
        //如果是QToolButton部件
        if(0==qstrcmp(objList.at(nIndex)->metaObject()->className(),"QToolButton"))
        {
            //连接pressed信号为slot_btnpress
            connect( ( ( QToolButton* )( objList.at( nIndex ) ) ), SIGNAL( pressed() ), this, SLOT( slot_btnpress() ) );
            //连接clicked信号为slot_btnclick
            connect( ( ( QToolButton* )( objList.at( nIndex ) ) ), SIGNAL( clicked() ), this, SLOT( slot_btnclick() ) );
            //设置顶部间距
            (  (QToolButton*)(objList.at(nIndex)))->setContentsMargins( 0, VALUE_DIS, 0, 0 );
        }
    }
}

//设置按钮不同状态下的图标
void TitleBar::SetBtnIcon( QToolButton *pBtn, eBtnMoustState state, bool bInit/*=false*/ )
{
    //获得图片路径
    QString strImagePath = GetBtnImagePath( pBtn, bInit );
    //创建QPixmap对象
    QPixmap objPixmap( strImagePath );
    //得到图像宽和高
    int nPixWidth = objPixmap.width();
    int nPixHeight = objPixmap.height();
    //如果状态不是无效值
    if( state != eBtnStateNone )
    {
        /*设置按钮图片
        按钮的图片是连续在一起的，如前1/4部分表示默认状态下的图片部分,接后的1/4部分表示鼠标移到按钮状态下的图片部分
        */
        pBtn->setIcon( objPixmap.copy(( nPixWidth/4 ) * ( state-1 ), 0, nPixWidth/4, nPixHeight ) );
        //设置按钮图片大小
        pBtn->setIconSize( QSize( nPixWidth/4, nPixHeight ) );
    }
}

//获得图片路径(固定值)  bInit=true表示按钮已经初始化
const QString TitleBar::GetBtnImagePath( QToolButton *pBtn, bool bInit/*=false*/ )
{
    QString strImagePath;
    //最小化
    if( mp_btn_min==pBtn )
    {
        strImagePath = "../image/btn_close.png";
    }
    //最大化/还原按钮，所以包括最大化和还原两张图片
    if( mp_btn_max ==pBtn )
    {
        //如果是初始设置或者主界面的最大化标志不为真(其中MainWindow::Instance()使用单例设计模式)
        if( bInit==true || ((QMainWindow *)parent())->isMaximized()==false ) //isMaximized表示父窗口是否最大化
        {
            //最大化按钮图片路径
            strImagePath = "../image/btn_close.png";
        }
        else
        {
            //还原按钮图片路径
            strImagePath = "../image/btn_close.png";
        }
    }
    //关闭按钮
    if( mp_btn_close == pBtn )
    {
        strImagePath = "../image/btn_close.png";
    }

    return strImagePath;
}

void TitleBar::resizeEvent( QResizeEvent *event )
{
    mp_label_icon->move( 0, 0 );
    //设置这个Label的最小大小为当前最适合的大小。即Label的最小面积就是现在的大小了
    mp_label_title->move( TITLE_H, 0 );

    qDebug()<<"按钮"<<mp_btn_max->width()<<mp_btn_max->height();
     mp_btn_close->move( width()-BUTTON_WIDTH-TITLE_SPACE, 0 );
     mp_btn_max->move( width()-BUTTON_WIDTH*2-TITLE_SPACE, 0 );
     mp_btn_min->move( width()-BUTTON_WIDTH*3-TITLE_SPACE, 0 );

     //mp_btn_analyse_interface->move( width()-BUTTON_WIDTH*5-TITLE_SPACE, 0 );
     mp_btn_config_interface->move( width()-BUTTON_WIDTH*4-TITLE_SPACE, 0 );
     mp_btn_backplay_interface->move( width()-BUTTON_WIDTH*5-TITLE_SPACE, 0 );
     mp_btn_monitor_interface->move( width()-BUTTON_WIDTH*5-TITLE_SPACE, 0 );
     //mp_label_version->move( width()-610, 0 );

    qDebug()<<" 标题栏大小 "<<width();

}

//如果鼠标双击的是标题栏中的按钮位置，那么不操作，返回false
bool TitleBar::is_click_in_btn( int x, int y )
{
    if( x>=mp_btn_close->x() && x<=mp_btn_close->x()+mp_btn_close->width() && y>=mp_btn_close->y() && y<=mp_btn_close->y()+mp_btn_close->height() ) {
        return false;
    } else if(x>=mp_btn_max->x() && x<=mp_btn_max->x()+mp_btn_max->width() && y>=mp_btn_max->y() && y<=mp_btn_max->y()+mp_btn_max->height() ) {
        return false;
    } else if( x>=mp_btn_min->x() && x<=mp_btn_min->x()+mp_btn_min->width() && y>=mp_btn_min->y() && y<=mp_btn_min->y()+mp_btn_min->height() ) {
        return false;
    } else if( x>=mp_btn_monitor_interface->x() && x<=mp_btn_monitor_interface->x()+mp_btn_monitor_interface->width() && y>=mp_btn_monitor_interface->y() && y<=mp_btn_monitor_interface->y()+mp_btn_monitor_interface->height() ) {
        return false;
    } else if( x>=mp_btn_config_interface->x() && x<=mp_btn_config_interface->x()+mp_btn_config_interface->width() && y>=mp_btn_config_interface->y() && y<=mp_btn_config_interface->y()+mp_btn_config_interface->height() ) {
        return false;
    } else if( x>=mp_btn_analyse_interface->x() && x<=mp_btn_analyse_interface->x()+mp_btn_analyse_interface->width() && y>=mp_btn_analyse_interface->y() && y<=mp_btn_analyse_interface->y()+mp_btn_analyse_interface->height() ) {
        return false;
    } else if( x>=mp_btn_backplay_interface->x() && x<=mp_btn_backplay_interface->x()+mp_btn_backplay_interface->width() && y>=mp_btn_backplay_interface->y() && y<=mp_btn_backplay_interface->y()+mp_btn_backplay_interface->height() ) {
        return false;
    } else {
        return true;
    }
}



void TitleBar::slot_btnpress()
{

}

//槽函数--slot_btnclick
void TitleBar::slot_btnclick()
{
    QToolButton *pBtn = ( QToolButton* )( sender() );
    if( pBtn == mp_btn_close )
    {
        emit signal_close();
    }
}


//鼠标按下事件
void TitleBar::mousePressEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton ) {
        if( event->y() < VALUE_DIS || event->x() < VALUE_DIS || rect().width() - event->x() < 5 ) {
            event->ignore();
            return;
        }
        m_pt_press = event->globalPos();
        mb_left_button_press = true;
    }
    event->ignore();
}
//鼠标移动事件
void TitleBar::mouseMoveEvent( QMouseEvent *event )
{
    if( mb_left_button_press ) {
        m_pt_move = event->globalPos();
        //移动主窗口
        VodPlayInterface *pMainWindow = (qobject_cast<VodPlayInterface *>( parent() ) );
        pMainWindow->move( pMainWindow->pos() + m_pt_move - m_pt_press );
        //重新设置m_ptPress;
        m_pt_press = m_pt_move;
    }

    event->ignore();
}
//鼠标释放事件
void TitleBar::mouseReleaseEvent( QMouseEvent *event )
{
    if( event->button() == Qt::LeftButton ) {
        mb_left_button_press = false;
    }
     event->ignore();
}




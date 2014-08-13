#include "titlebar.h"
#include <Qt>
#include <QPainter>
#include <QDebug>
#include <QPixmap>
#include <QStyle>
#include <QBitmap>

#define VALUE_DIS 50
#define TITLE_H 50

TitleBar::TitleBar(QWidget *parent) :
    QWidget(parent)
    ,mp_label_icon( NULL )
    ,mp_label_title( NULL )
    ,mp_btn_min( NULL )
    ,mp_btn_close( NULL )
{
    mb_left_button_press = false;
    create_widget();

    connect( mp_btn_min, SIGNAL( clicked() ), parent, SLOT( showMinimized() ) );
    //发送关闭主窗口的信号
    connect( mp_btn_close, SIGNAL( clicked() ), parent, SLOT( close() ) );
}

//创建子部件
void TitleBar::create_widget()
{
    //设置字号
    QFont ft;
    ft.setPointSize( 16 );
    //ui.label_4->setFont(ft);

    //图像标签--logo
    mp_label_icon = new QLabel( this );
    QPixmap objPixmap( ":image/res/caption_logo.bmp" );
    QBitmap mask = objPixmap.createMaskFromColor( QColor( 255, 255, 255 ), Qt::MaskInColor );
    objPixmap.setMask( mask );
    mp_label_icon->setPixmap( objPixmap.scaled( TITLE_H*4, TITLE_H ) );
    mp_label_icon->setMinimumSize( mp_label_icon->sizeHint() );

    //文本标签--标题
    mp_label_title = new QLabel( this );
    mp_label_title->setText( QString( "阀厅红外在线监测系统" ) );
    mp_label_title->setMinimumSize( mp_label_title->sizeHint() );
    mp_label_title->setFont( ft );

    //按钮--最小化
    mp_btn_min = new QToolButton(this);
    set_btn_icon( mp_btn_min, eBtnStateDefault, true);
    //按钮--关闭
    mp_btn_close = new QToolButton(this);
    set_btn_icon(mp_btn_close,eBtnStateDefault,true);

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
            (  (QToolButton*)(objList.at(nIndex)))->setContentsMargins(0,VALUE_DIS,0,0);
        }
    }
}

//鼠标按下事件
void TitleBar::mousePressEvent(QMouseEvent *event)
{
    if (   event->button() == Qt::LeftButton)
    {
     if(event->y()<VALUE_DIS||event->x()<VALUE_DIS||rect().width()-event->x()<5)
        {
            event->ignore();
            return;
        }
        m_pt_press = event->globalPos();
        mb_left_button_press = true;
    }
    event->ignore();
}
//鼠标移动事件
void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if(mb_left_button_press)
    {
        m_pt_move = event->globalPos();
        //重新设置m_ptPress;
        m_pt_press = m_pt_move;
    }
    event->ignore();
}
//鼠标释放事件
void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mb_left_button_press = false;
    }
     event->ignore();
}

//设置按钮不同状态下的图标
void TitleBar::set_btn_icon( QToolButton *pBtn, eBtnMoustState state, bool bInit/*=false*/)
{
    //获得图片路径
    QString strImagePath = get_btn_image_path( pBtn, bInit );
    //创建QPixmap对象
    QPixmap objPixmap(strImagePath);
    //得到图像宽和高
    int nPixWidth = objPixmap.width();
    int nPixHeight = objPixmap.height();
    //如果状态不是无效值
    if(state!=eBtnStateNone)
    {
        /*设置按钮图片
        按钮的图片是连续在一起的，如前1/4部分表示默认状态下的图片部分,接后的1/4部分表示鼠标移到按钮状态下的图片部分
        */
        pBtn->setIcon(objPixmap.copy((nPixWidth/4)*(state-1),0,nPixWidth/4,nPixHeight));
        //设置按钮图片大小
        pBtn->setIconSize(QSize(nPixWidth/4,nPixHeight));
    }
}

//获得图片路径(固定值)
const QString TitleBar::get_btn_image_path(QToolButton *pBtn,bool bInit/*=false*/)
{
    QString strImagePath;

    //最小化
    if( mp_btn_min==pBtn)
    {
        strImagePath = ":image/res/caption_min.bmp";
    }

    //关闭按钮
    if( mp_btn_close == pBtn)
    {
        strImagePath = ":image/res/caption_close.bmp";
    }
    return strImagePath;
}

void TitleBar::resizeEvent(QResizeEvent *event)
{
    mp_label_icon->move( 0, 0 );
    //setFrameStyle 去除label边框，但实际没有发挥作用
    mp_label_icon->setFrameStyle( QFrame::NoFrame );

    mp_label_title->move( TITLE_H*5, 0 );

    mp_btn_close->move( width()-36, 0 );
    mp_btn_min->move( width()-72, 0 );
    qDebug()<<"TitleBar::resizeEvent"<<width()<<height();

}

void TitleBar::paintEvent(QPaintEvent *event)
{


}

void TitleBar::slot_btnpress()
{

}

//槽函数--slot_btnclick
void TitleBar::slot_btnclick()
{
    QToolButton *pBtn = (QToolButton*)(sender());
    if(pBtn==mp_btn_min)
    {
        emit signal_min();
    }

    if(pBtn==mp_btn_close)
    {
        emit signal_close();
    }
}


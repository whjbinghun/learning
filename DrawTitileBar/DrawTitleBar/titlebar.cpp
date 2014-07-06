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
{
    mb_left_button_press = false;
    CreateWidget();

    connect(mp_btn_skin, SIGNAL(clicked()), this, SIGNAL(showSkin()));
    //connect(main_menu_button, SIGNAL(clicked()), this, SIGNAL(showMainMenu()));
    connect(mp_btn_min, SIGNAL(clicked()), this, SIGNAL(showMin()));
    connect(mp_btn_max, SIGNAL(clicked()), this, SIGNAL(showMax()));
    //发送关闭主窗口的信号
    connect(mp_btn_close, SIGNAL(clicked()), parent, SIGNAL(close_window()) );
    //connect( this, SIGNAL( closeWidget() ), this, SLOT(  ) );
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
    QPixmap objPixmap( "../image/360AboutLogo.png");
    QBitmap mask = objPixmap.createMaskFromColor( QColor( 255, 255, 255 ), Qt::MaskInColor );
    objPixmap.setMask( mask );
    mp_label_icon->setPixmap( objPixmap.scaled( TITLE_H, TITLE_H ) );
    mp_label_icon->setMinimumSize( mp_label_icon->sizeHint() );


    //setFrameStyle 去除label边框，但实际没有发挥作用
    //mp_label_icon->setFrameStyle(QFrame::NoFrame);


    //文本标签--标题
    mp_label_title = new QLabel(this);
    mp_label_title->setText(QString("360 Safe Guard V8.5"));
    mp_label_title->setMinimumSize( mp_label_title->sizeHint() );
    //mp_label_title->setFrameStyle( QFrame::NoFrame );
    mp_label_title->setFont( ft );


    //文本标签--样式版本
    mp_label_version = new QLabel(this);
    mp_label_version->setText(QString("Use Class Style"));
    //设置鼠标形状
    mp_label_version->setCursor(Qt::PointingHandCursor);
    //mp_label_version->setFrameStyle(QFrame::NoFrame);
    //mp_label_version->setFrameShape();

    //按钮--更换皮肤
    mp_btn_skin = new QToolButton(this);
    //设置初始图片
    SetBtnIcon(mp_btn_skin, eBtnStateDefault, true);
    //按钮--菜单
    mp_btn_menu = new QToolButton(this);
    SetBtnIcon(mp_btn_menu,eBtnStateDefault,true);
    //按钮--最小化
    mp_btn_min = new QToolButton(this);
    SetBtnIcon( mp_btn_min, eBtnStateDefault, true);
    //按钮--最大化/还原
    mp_btn_max = new QToolButton(this);
    SetBtnIcon(mp_btn_max,eBtnStateDefault,true);
    //按钮--关闭
    mp_btn_close = new QToolButton(this);
    SetBtnIcon(mp_btn_close,eBtnStateDefault,true);
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


//设置子部件样式(qss)
void TitleBar::SetWidgetStyle()
{
    //设置标签的文本颜色，大小等以及按钮的边框
    setStyleSheet("QLabel{color:#CCCCCC;font-size:12px;font-weight:bold;}QToolButton{border:0px;}");
    //设置左边距
    mp_label_title->setStyleSheet("margin-left:6px;");
    //设置右边距以及鼠标移上去时的文本颜色
    mp_label_version->setStyleSheet("QLabel{margin-right:10px;}QLabel:hover{color:#00AA00;}");
}

//创建设置布局
void TitleBar::CreateLayout()
{
    //水平布局
    mp_layout = new QHBoxLayout(this);
    //添加部件
    mp_layout->addWidget(mp_label_icon);
    mp_layout->addWidget(mp_label_title);
    //添加伸缩项
    mp_layout->addStretch(1);
    //添加部件
    mp_layout->addWidget(mp_label_version);
    mp_layout->addWidget(mp_btn_skin);
    mp_layout->addWidget(mp_btn_menu);
    mp_layout->addWidget(mp_btn_min);
    mp_layout->addWidget(mp_btn_max);
    mp_layout->addWidget(mp_btn_close);
    //设置Margin
    mp_layout->setContentsMargins(0,0,VALUE_DIS,0);
    //设置部件之间的space
    mp_layout->setSpacing(0);
    setLayout(mp_layout);
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
        //移动主窗口
        //DrawTitleBar *pMainWindow = (qobject_cast<DrawTitleBar *>(parent()));
        //pMainWindow->move(pMainWindow->pos()+m_pt_move-mp_press);
        //重新设置m_ptPress;
        m_pt_press = m_pt_move;
    }
    /*else if( mp_label_version ) {
        //在标签中设置字体颜色
        mp_label_version->setStyleSheet("color: green");
        update();
    }*/
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
void TitleBar::SetBtnIcon( QToolButton *pBtn, eBtnMoustState state, bool bInit/*=false*/)
{
    //获得图片路径
    QString strImagePath = GetBtnImagePath( pBtn, bInit );
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
const QString TitleBar::GetBtnImagePath(QToolButton *pBtn,bool bInit/*=false*/)
{
    QString strImagePath;
    //皮肤按钮
    if( mp_btn_skin == pBtn )
    {
        strImagePath = "../image/SkinButtom.png";
    }
    //菜单按钮
    if( mp_btn_menu == pBtn )
    {
        strImagePath = "../image/title_bar_menu.png";
    }
    //最小化
    if( mp_btn_min==pBtn)
    {
        strImagePath = "../image/sys_button_min.png";
    }
    //最大化/还原按钮，所以包括最大化和还原两张图片
    if( mp_btn_max ==pBtn)
    {
        //如果是初始设置或者主界面的最大化标志不为真(其中MainWindow::Instance()使用单例设计模式)
        /*if(bInit==true || DrawTitleBar::Instance()->GetMaxWin()==false)
        {
            //最大化按钮图片路径
            strImagePath = "../image/sys_button_max.png";
        }
        else
        {
            //还原按钮图片路径
            strImagePath = "../image/sys_button_restore.png";
        }*/
    }
    //关闭按钮
    if( mp_btn_close == pBtn)
    {
        strImagePath = "../image/btn_close.png";
    }
    return strImagePath;
}

void TitleBar::resizeEvent(QResizeEvent *event)
{
    mp_label_icon->move( 0, 0 );
    //mp_label_icon->resize( 50, 50 );
    //setFrameStyle 去除label边框，但实际没有发挥作用
    mp_label_icon->setFrameStyle( QFrame::NoFrame );

    mp_label_title->move( 35, 0 );
    //设置这个Label的最小大小为当前最适合的大小。即Label的最小面积就是现在的大小了
    //mp_label_title->setMinimumSize( mp_label_title->sizeHint() );
    //mp_label_title->resize( 50, 10);

    mp_label_version->move( 500, 0 );

    mp_btn_close->move( width()-60, 0 );


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
    if(pBtn==mp_btn_max)
    {
        emit signal_max_restore();
    }
    if(pBtn==mp_btn_close)
    {
        emit signal_close();
    }
}

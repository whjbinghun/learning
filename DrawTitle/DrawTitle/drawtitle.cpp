#include "drawtitle.h"
#include "ui_drawtitle.h"
#include <QMouseEvent>


#include <QDebug>

#define VALUE_DIS 20
#define TITLE_H 200

DrawTitle::DrawTitle(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DrawTitle)
{
    ui->setupUi(this);
    //隐藏标题栏
   // setWindowFlags (Qt::CustomizeWindowHint);
    setWindowFlags(Qt::FramelessWindowHint);
    //CreateWidget();
}

DrawTitle::~DrawTitle()
{
    delete ui;
}

//创建子部件
void DrawTitle::CreateWidget()
{
    //设置标题栏名字
    /*setWindowTitle(tr("your title"));
    //在标题栏放入360logo
    QPixmap objPixmap("360AboutLogo.png");
    DrawTitle::setWindowIcon( QIcon( objPixmap ) );
    //图像标签--logo
    mp_label_icon = new QLabel(this);
   // QPixmap objPixmap("360AboutLogo.png");
    mp_label_icon->setPixmap( QPixmap( "360AboutLogo.png" ) );
    //mp_label_icon->setPixmap( objPixmap.scaled( TITLE_H, TITLE_H ) );
    qDebug()<<"111111111";

    //文本标签--标题
    /*mp_label_title = new QLabel(this);
    mp_label_title->setText(QString("360 Safe Guard V8.5"));
    //文本标签--样式版本
    mp_label_version = new QLabel(this);
    mp_label_version->setText(QString("Use Class Style"));
    //设置鼠标形状
    mp_label_version->setCursor(Qt::PointingHandCursor);
    //按钮--更换皮肤
    mp_btn_skin = new QToolButton(this);
    //设置初始图片
    SetBtnIcon(mp_btn_skin,eBtnStateDefault,true);
    //按钮--菜单
    mp_btn_menu = new QToolButton(this);
    SetBtnIcon(mp_btn_menu,eBtnStateDefault,true);
    //按钮--最小化
    mp_btn_min = new QToolButton(this);
    SetBtnIcon(mp_btn_min,eBtnStateDefault,true);
    //按钮--最大化/还原
    m_pBtnMax = new QToolButton(this);
    SetBtnIcon(m_pBtnMax,eBtnStateDefault,true);
    //按钮--关闭
    m_pBtnClose = new QToolButton(this);
    SetBtnIcon(m_pBtnClose,eBtnStateDefault,true);
    //获得子部件
    const QObjectList &objList = children();
    for(int nIndex=0; nIndex<objList.count();++nIndex)
    {
        //设置子部件的MouseTracking属性
        ((QWidget*)(objList.at(nIndex)))->setMouseTracking(true);
        //如果是QToolButton部件
        if(0==qstrcmp(objList.at(nIndex)->metaObject()->className(),"QToolButton"))
        {
            //连接pressed信号为slot_btnpress
            connect(((QToolButton*)(objList.at(nIndex))),SIGNAL(pressed()),this,SLOT(slot_btnpress()));
            //连接clicked信号为slot_btnclick
            connect(((QToolButton*)(objList.at(nIndex))),SIGNAL(clicked()),this,SLOT(slot_btnclick()));
            //设置顶部间距
            ((QToolButton*)(objList.at(nIndex)))->setContentsMargins(0,20,0,0);
        }
    }*/
}

//子部件创建完之后，就要设置这些子部件的基本样式了，我使用qss样式表对其进行样式设置，当然还有其他方法。
//设置子部件样式(qss)
void DrawTitle::SetWidgetStyle()
{
    //设置标签的文本颜色，大小等以及按钮的边框
    setStyleSheet("QLabel{color:#CCCCCC;font-size:12px;font-weight:bold;}QToolButton{border:0px;}");
    //设置左边距
    mp_label_title->setStyleSheet("margin-left:6px;");
    //设置右边距以及鼠标移上去时的文本颜色
    mp_label_version->setStyleSheet("QLabel{margin-right:10px;}QLabel:hover{color:#00AA00;}");
}

//最后就是创建布局管理器，把这些子部件加入到布局管理器中，我使用水平布局管理器，如下代码所示：
//创建设置布局
void DrawTitle::CreateLayout()
{
    //水平布局
    m_pLayout = new QHBoxLayout(this);
    //添加部件
    m_pLayout->addWidget(mp_label_icon);
    m_pLayout->addWidget(mp_label_title);
    //添加伸缩项
    m_pLayout->addStretch(1);
    //添加部件
    m_pLayout->addWidget(mp_label_version);
    m_pLayout->addWidget(mp_btn_skin);
    m_pLayout->addWidget(mp_btn_menu);
    m_pLayout->addWidget(mp_btn_min);
    m_pLayout->addWidget(m_pBtnMax);
    m_pLayout->addWidget(m_pBtnClose);
    //设置Margin
    m_pLayout->setContentsMargins(0,0,VALUE_DIS,0);
    //设置部件之间的space
    m_pLayout->setSpacing(0);
    setLayout(m_pLayout);
}




      //而bInit表示是否是初始化设置，因为在SetBtnIcon函数里需要获得主界面最大化标志值，而这时候主界面窗口构造函数还没完成，同时在SetBtnIcon函数里又需要获得主界面窗口对象，因此会矛盾，所以使用了bInit标志值进行区分。
      //SetBtnIcon函数的定义如下代码所示：
//设置按钮不同状态下的图标
void DrawTitle::SetBtnIcon(QToolButton *pBtn,eBtnMoustState state,bool bInit/*=false*/)
{
    //获得图片路径
    QString strImagePath = GetBtnImagePath(pBtn,bInit);
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
const QString DrawTitle::GetBtnImagePath(QToolButton *pBtn,bool bInit/*=false*/)
{
    QString strImagePath;
    //皮肤按钮
    if(mp_btn_skin==pBtn)
    {
        strImagePath = ":/image/SkinButtom.png";
    }
    //菜单按钮
    if(mp_btn_menu==pBtn)
    {
        strImagePath = ":/image/title_bar_menu.png";
    }
    //最小化
    if(mp_btn_min==pBtn)
    {
        strImagePath = ":/image/sys_button_min.png";
    }
    //最大化/还原按钮，所以包括最大化和还原两张图片
    if(m_pBtnMax==pBtn)
    {
        //如果是初始设置或者主界面的最大化标志不为真(其中MainWindow::Instance()使用单例设计模式)
        /*if(bInit==true || MainWindow::Instance()->GetMaxWin()==false)
        {
            //最大化按钮图片路径
            strImagePath = ":/image/sys_button_max.png";
        }
        else
        {
            //还原按钮图片路径
            strImagePath = ":/image/sys_button_restore.png";
        }*/
    }
    //关闭按钮
    if(m_pBtnClose==pBtn)
    {
        strImagePath = ":/image/sys_button_close.png";
    }
    return strImagePath;
}

//二、设置按钮其他效果
//各位在使用360安全卫士的时候，把鼠标移到关闭按钮上或者使用鼠标按下关闭按钮，其呈现不同的图片以示区分，当然其他按钮也一样。那么是不是也和工具栏按钮一样，子类化一个按钮了？不需要。使用事件过滤器，在标题栏部件中进行事件判断和目标判断即可。
//首先是创建事件过滤器，代码如下所示：
//创建事件过滤器
void DrawTitle::CreateEventFiter()
{
    mp_btn_skin->installEventFilter(this);
    mp_btn_menu->installEventFilter(this);
    mp_btn_min->installEventFilter(this);
    m_pBtnMax->installEventFilter(this);
    m_pBtnClose->installEventFilter(this);
}

//然后在标题栏部件中重写eventFilter函数即可，代码如下：
//事件过滤
bool DrawTitle::eventFilter(QObject *obj, QEvent *event)
{
    //按钮状态
    eBtnMoustState eState = eBtnStateNone;
    //判断事件类型--QEvent::Enter
    if (event->type() == QEvent::Enter)
    {
        eState = eBtnStateHover;
    }
    //判断事件类型--QEvent::Leave
    if (event->type() == QEvent::Leave)
    {
        eState = eBtnStateDefault;
    }
    //判断事件类型--QEvent::MouseButtonPress
    if (event->type() == QEvent::MouseButtonPress && ((QMouseEvent*)(event))->button()== Qt::LeftButton)
    {
        eState = eBtnStatePress;
    }
    //判断目标
    if(mp_btn_skin==obj || mp_btn_menu==obj || mp_btn_min==obj || m_pBtnMax==obj || m_pBtnClose==obj)
    {
        //如果状态有效
        if(eState != eBtnStateNone)
        {
            //根据状态设置按钮图标
            SetBtnIcon((QToolButton *)obj,eState, true);
            return false;
        }
    }
    return QWidget::eventFilter(obj,event);
}

// 即根据事件类型设置按钮状态；最后在各个按钮的click槽函数中实现相应功能即可，如窗口关闭，最大化等。
//槽函数--slot_btnclick
void DrawTitle::slot_btnclick()
{
    QToolButton *pBtn = (QToolButton*)(sender());
    if(pBtn==mp_btn_min)
    {
        //emit signal_min();
    }
    if(pBtn==m_pBtnMax)
    {
        //emit signal_maxrestore();
    }
    if(pBtn==m_pBtnClose)
    {
        //emit signal_close();
    }
}

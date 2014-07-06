#include "drawtitlebar.h"
#include "ui_drawtitlebar.h"
#include <QBitmap>
#include <QPainter>
#include <QDesktopWidget>
#include <QToolButton>
#include <QDebug>

//改变鼠标状态距离边框的位置
#define MOUSE_SPACE 5

DrawTitleBar::DrawTitleBar(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DrawTitleBar)
{
    ui->setupUi(this);
    //隐藏标题栏
    setWindowFlags (Qt::FramelessWindowHint);
    //创建标题栏  类
    mp_title_bar = new TitleBar(this);
    //mp_title_bar->setWindowIcon( QIcon( QPixmap( "../image/frame.jpg" ) ) );
    //mp_title_bar->resize( ui->centralWidget->width(), 50 );
    member_init();


    setMinimumWidth(850);
    setMinimumHeight(600);

    //获取destop窗口的分辨率
    //m_rect_restore_window = new QRect( 0, 0, QApplication::desktop()->width(), QApplication::desktop()->height() );

}

DrawTitleBar::~DrawTitleBar()
{
    delete ui;
}

void DrawTitleBar::member_init() {
    mb_max_win = 0;
    ui->centralWidget->setMouseTracking( true );
    mp_title_bar->setMouseTracking( true );
    setMouseTracking(true);

    mp_mouse_pos_label = new QLabel( this ) ;


    //生成一张位图
    /*QBitmap objBitmap( size() );
    //QPainter用于在位图上绘画
    QPainter painter( &objBitmap );
    //填充位图矩形框(用白色填充)
    //painter.fillRect( rect(), Qt::white );
    painter.setBrush( QColor( 0, 0, 0 ) );
    //在位图上画圆角矩形(用黑色填充)
    painter.drawRoundedRect( this->rect(), 10, 10 );
    //使用setmask过滤即可
    setMask( objBitmap );
    */
}


//鼠标按下事件
void DrawTitleBar::mousePressEvent(QMouseEvent *event)
{
    if ( event->button() == Qt::LeftButton )
    {
        m_pt_press_global = event->globalPos();
        mb_left_btn_press = true;
    }
}
//鼠标移动事件
void DrawTitleBar::mouseMoveEvent( QMouseEvent *event )
{
    if( !mb_left_btn_press )
    {
        m_eDirection = PointValid( event->x(), event->y() );
        SetCursorStyle( m_eDirection );
    }
    else
    {
        int nx_global = event->globalX();
        int ny_global = event->globalY();
        //SetDrayMove( nx_global, ny_global, m_eDirection );
        SetDrayMove( event, nx_global, ny_global, m_eDirection );
        m_pt_press_global = QPoint( nx_global, ny_global );
    }
    mp_mouse_pos_label->setText( "("+QString::number( event->x() ) +", "+QString::number( event->y() ) +")" ) ;
}
//鼠标释放事件
void DrawTitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mb_left_btn_press = false;
        m_eDirection = eNone;
    }
 }
//鼠标双击事件
void DrawTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->y()<= mp_title_bar->height())
    {
        if(!mb_max_win)
        {
            m_rect_restore_window = geometry();
            setGeometry(qApp->desktop()->availableGeometry());
        }
        else
        {
            setGeometry(m_rect_restore_window);
        }
        mb_max_win = !mb_max_win;
    }
}

void DrawTitleBar::resizeEvent(QResizeEvent *event)
{
    mp_title_bar->move( 0, 0 );
    mp_title_bar->resize( ui->centralWidget->width(), 50 );
    qDebug()<<"主窗口的宽度："<<ui->centralWidget->width()<<ui->centralWidget->height();

    mp_mouse_pos_label->move( ui->centralWidget->width()/2, ui->centralWidget->height()/2 );

    resize( ui->centralWidget->width(), ui->centralWidget->height() );

    //生成一张位图
    QBitmap objBitmap( size() );
    //QPainter用于在位图上绘画
    QPainter painter( &objBitmap );
    //填充位图矩形框(用白色填充)
    //painter.fillRect( rect(), Qt::white );
    painter.setBrush( QColor( 0, 0, 0 ) );
    //在位图上画圆角矩形(用黑色填充)
    painter.drawRoundedRect( this->rect(), 10, 10 );
    //使用setmask过滤即可
    setMask( objBitmap );


}

void DrawTitleBar::paintEvent( QPaintEvent *event )
{
    QPainter painter(this);
    //标题栏背景图片
    painter.drawPixmap( 0, 0, ui->centralWidget->width(), 50, QPixmap("../image/frame.jpg"));
    //整个窗口的背景图片
    //painter.drawPixmap( 0, 0, ui->centralWidget->width(), ui->centralWidget->height(), QPixmap("../image/frame.jpg"));

}
//设置鼠标样式
void DrawTitleBar::SetCursorStyle( Direction direction )
{
    //设置上下左右以及右上、右下、左上、坐下的鼠标形状
    switch( direction )
    {
    case eTop:
    case eBottom:
        setCursor( Qt::SizeVerCursor );
        break;
    case eRight:
    case eLeft:
        setCursor( Qt::SizeHorCursor );
        break;
    case eTopRight:
    case eBottomLeft:
        setCursor( Qt::SizeBDiagCursor );
        break;
    case eRightBottom:
    case eLeftTop:
        setCursor( Qt::SizeFDiagCursor );
        break;
    default:
        setCursor( Qt::ArrowCursor );
        break;
    }
}
//设置鼠标拖动的窗口位置信息
void DrawTitleBar::SetDrayMove( int nx_global, int ny_global, DrawTitleBar::Direction direction )
{
    //计算偏差
    int ndX = nx_global - m_pt_press_global.x();
    int ndY = ny_global - m_pt_press_global.y();
    //获得主窗口位置信息
    QRect rectWindow = geometry();

    if( direction & eTop )
    {
        rectWindow.setTop( rectWindow.top()+ndY );
        //rectWindow.y1 = rectWindow.y1 + ndY;
    }
    if( direction & eRight )
    {
        rectWindow.setRight( rectWindow.right()+ndX );
        //rectWindow.x2 = rectWindow.x2 + ndX;
    }
    if( direction & eBottom )
    {
        rectWindow.setBottom( rectWindow.bottom()+ndY );
        //rectWindow.y2 = rectWindow.y2 + ndY;
    }
    if( direction & eLeft )
    {
        rectWindow.setLeft( rectWindow.left()+ndX );
        //rectWindow.x1 = rectWindow.x1 + ndX;
    }
    if( rectWindow.width()< minimumWidth() || rectWindow.height()<minimumHeight() )
    {
        return;
    }
    //重新设置窗口位置为新位置信息
    setGeometry( rectWindow );
}

void DrawTitleBar::SetDrayMove(QMouseEvent *event, int nx_global, int ny_global, DrawTitleBar::Direction direction)
{
    //计算偏差
    int ndX = nx_global - m_pt_press_global.x();
    int ndY = ny_global - m_pt_press_global.y();
    qDebug()<<"0000"<<nx_global<<ny_global<<m_pt_press_global.x()<<m_pt_press_global.y()<<ndX<<ndY;
    //获得主窗口位置信息
    QRect rectWindow = geometry();
    if( direction == eTop ) {
       //ndY>0 往下移动
       if ( ndY > 0 ) {
            rectWindow.setTop( rectWindow.top()+ndY );
            if ( rectWindow.height()<=minimumHeight() ) {
                return ;
            }
        } else {
           int pt_y = ny_global;
            int rect_top = rectWindow.top();
            if ( pt_y > rect_top ) {
                return;
            } else {
                rectWindow.setTop( rectWindow.top()+ndY );
            }
        }
    } else if( direction == eBottom ) {
        if ( ndY > 0 ) {
            int pt_y = ny_global;
            int rect_bottom = rectWindow.bottom();
            if ( pt_y < rect_bottom ) {
                return;
            } else {
                rectWindow.setBottom( rectWindow.bottom()+ndY );
            }


         } else {
            rectWindow.setBottom( rectWindow.bottom()+ndY );
            if ( rectWindow.height()<=minimumHeight() ) {
                return ;
            }
         }
    } else if( direction == eRight ) {
        //ndX>0往右
        if ( ndX > 0 ) {
            int pt_x = nx_global;
            int rect_right = rectWindow.right();
            if ( pt_x < rect_right ) {
                return;
            } else {
                rectWindow.setRight( rectWindow.right()+ndX );
            }
         } else {
            rectWindow.setRight( rectWindow.right()+ndX );
            if ( rectWindow.width()<=minimumWidth() ) {
                return ;
            }
         }
    } else if( direction == eLeft ) {
        if( ndX>0 ) {
            rectWindow.setLeft( rectWindow.left()+ndX );
            if( rectWindow.width()<= minimumWidth() ){
                return;
            }
        } else {
            int pt_x = nx_global;
            int rect_left = rectWindow.left();
            if( pt_x > rect_left ) {
                return ;
            } else {
                rectWindow.setLeft( rectWindow.left()+ndX );
            }
        }
    } else if( direction == eTopRight ) {
        //ndY>0 往下移动
        bool b_top_change = false;
        bool b_right_change = false;
        if ( ndY > 0 ) {
            QRect rct_temp = rectWindow;
            rct_temp.setTop( rct_temp.top()+ndY );
            if ( rct_temp.height()<=minimumHeight() ) {

            } else {
                //rectWindow = rct_temp;
                b_top_change = true;
            }
        } else {
            int pt_y = ny_global;
             int rect_top = rectWindow.top();
             if ( pt_y > rect_top ) {

             } else {

                 b_top_change = true;//rectWindow.setTop( rectWindow.top()+ndY );
             }
        }

        if( ndX >0 ) {
            int pt_x = nx_global;
            int rect_right = rectWindow.right();
            if ( pt_x < rect_right ) {

            } else {
                b_right_change = true;
                //rectWindow.setRight( rectWindow.right()+ndX );
            }

        } else {
            QRect rct_temp = rectWindow;
            rct_temp.setRight( rct_temp.right()+ndX );
            if ( rct_temp.width()<=minimumWidth() ) {

            } else {
                b_right_change = true;
                //rectWindow = rct_temp;
            }
        }

        if ( b_top_change ) {
            rectWindow.setTop( rectWindow.top()+ndY );
        }
        if ( b_right_change ) {
            rectWindow.setRight( rectWindow.right()+ndX );
        }

    } else if( direction == eBottomLeft ) {
        if ( ndY > 0 ) {
            int pt_y = ny_global;
            int rect_bottom = rectWindow.bottom();
            if ( pt_y < rect_bottom ) {
                //return;
            } else {
                rectWindow.setBottom( rectWindow.bottom()+ndY );
            }


         } else {
            QRect rect_temp = rectWindow;
            rect_temp.setBottom( rect_temp.bottom()+ndY );
            if ( rect_temp.height()<=minimumHeight() ) {
                //return ;
            } else {
                rectWindow = rect_temp;
            }
         }
        if( ndX>0 ) {
            QRect rect_temp = rectWindow;
            rect_temp.setLeft( rect_temp.left()+ndX );
            if( rect_temp.width()<= minimumWidth() ){
                //return;
            } else {
                rectWindow = rect_temp;
            }
        } else {
            int pt_x = nx_global;
            int rect_left = rectWindow.left();
            if( pt_x > rect_left ) {
                //return ;
            } else {
                rectWindow.setLeft( rectWindow.left()+ndX );
            }
        }

    } else if( direction == eRightBottom ) {
        //ndX>0往右
        if ( ndX > 0 ) {
            int pt_x = nx_global;
            int rect_right = rectWindow.right();
            if ( pt_x < rect_right ) {
                //return;
            } else {
                rectWindow.setRight( rectWindow.right()+ndX );
            }
         } else {
            QRect rect_temp = rectWindow;
            rect_temp.setRight( rect_temp.right()+ndX );
            if ( rect_temp.width()<=minimumWidth() ) {
                //return ;
            } else {
                rectWindow = rect_temp;
            }
         }
         if ( ndY > 0 ) {
            int pt_y = ny_global;
            int rect_bottom = rectWindow.bottom();
            if ( pt_y < rect_bottom ) {
                //return;
            } else {
                rectWindow.setBottom( rectWindow.bottom()+ndY );
            }

         } else {
            QRect rect_temp = rectWindow;
            rect_temp.setBottom( rect_temp.bottom()+ndY );
            if ( rect_temp.height()<=minimumHeight() ) {
                //return ;
            } else {
                rectWindow = rect_temp;
            }
         }

    } else if( direction == eLeftTop ) {
        //ndY>0 往下移动
        if ( ndY > 0 ) {
            QRect rect_temp = rectWindow;
             rect_temp.setTop( rect_temp.top()+ndY );
             if ( rect_temp.height()<=minimumHeight() ) {
                 //return ;
             } else {
                 rectWindow = rect_temp;
             }
         } else {
            int pt_y = ny_global;
             int rect_top = rectWindow.top();
             if ( pt_y > rect_top ) {
                 //return;
             } else {
                 rectWindow.setTop( rectWindow.top()+ndY );
             }
         }
        if( ndX>0 ) {
            QRect rect_temp = rectWindow;
            rect_temp.setLeft( rect_temp.left()+ndX );
            if( rect_temp.width()<= minimumWidth() ){
                //return;
            } else {
                rectWindow = rect_temp;
            }
        } else {
            int pt_x = nx_global;
            int rect_left = rectWindow.left();
            if( pt_x > rect_left ) {
                //return ;
            } else {
                rectWindow.setLeft( rectWindow.left()+ndX );
            }
        }
    } /*else if( ny_global>=rectWindow.top() && ny_global<=rectWindow.top()+50 && nx_global>=rectWindow.left()+5 && nx_global<=rectWindow.right()-5 ) {
        if( direction == eTop ) {
            rectWindow.setTop( rectWindow.top()+ndY );
        }
        if( direction == eBottom ) {
            rectWindow.setBottom( rectWindow.bottom()+ndY );
            qDebug()<<"2222"<<ndY<<rectWindow.bottom()<<rectWindow;
        }
        if( direction == eRight ) {
            rectWindow.setRight( rectWindow.right()+ndX );
            qDebug()<<"33333"<<ndX<<rectWindow.right()<<rectWindow;
        }
        if( direction == eLeft ) {
            rectWindow.setLeft( rectWindow.left()+ndX );
            qDebug()<<"444444"<<ndX<<rectWindow.left()<<rectWindow;
        }
        if( rectWindow.width()< minimumWidth() || rectWindow.height()<minimumHeight() ) {
            return;
        }
    }*/


    setGeometry( rectWindow );

}

void DrawTitleBar::init_mini_window( QRect rectWindow ) {
    if( rectWindow.width()< minimumWidth() || rectWindow.height()<minimumHeight() ) {
        return ;
    }
}

//自己写的函数，判断鼠标是否在一个按钮区域内
bool DrawTitleBar::enterBtn(QPoint pp, QToolButton *btn)
{
    int height = btn->height();
    int width = btn->width();
    QPoint btnMinPos = btn->pos();
    QPoint btnMaxPos = btn->pos();
    btnMaxPos.setX(btn->pos().x()+width);
    btnMaxPos.setY(btn->pos().y()+height);
    if(pp.x() >= btnMinPos.x() && pp.y() >= btnMinPos.y()
        && pp.x() <= btnMaxPos.x() && pp.y() <= btnMaxPos.y())
        return true;
    else
        return false;
}

DrawTitleBar::Direction DrawTitleBar::PointValid( int x, int y ) {
    Direction direction;
    if( x>width()-5 && x<=width()+5 && y<5 && y>=-5 ) {
        direction = eTopRight;
    } else if( x<5 && x>=-5 && y>height()-5 && y<=height()+5 ) {
        direction = eBottomLeft;
    } else if( x>width()-5 && x<=width()+5 && y>height()-5 && y<=height()+5 ) {
        direction = eRightBottom;
    } else if( x<5 && x>=-5 && y<5 && y>=-5 ) {
        direction = eLeftTop;
    } else if( x>=5 && x<=width()-5 && y>=-5 && y<5 ) {
        direction = eTop;
    } else if( x>=5 &&x<=width()-5 && y>height()-5 && y<=height()+5 ) {
        direction = eBottom;
    } else if( x>=width()-5 && x<width()+5 && y>=5 && y<=height()-5 ) {
        direction = eRight;
    } else if( x>=-5 && x<5 && y>=5 && y<=height()-5 ) {
        direction = eLeft;
    }

    return direction;
}


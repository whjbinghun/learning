#include "titlebar.h"
#include <QPainter>
#include <QMouseEvent>

#define BUTTON_WIDTH 32
#define BUTTON_HEIGHT 23
#define TITLE_SPACE 10

TitleBar::TitleBar(QWidget *parent) :
    QWidget(parent)
{
    m_pixmap_backgroud = QPixmap(":image/image/caption_back.bmp");
    m_pixmap_logo = QPixmap( ":image/image/caption_logo.bmp" );

    m_font_title.setPointSize( 32 );

    setStyleSheet("QLabel{color:#CCCCCC;font-size:32px;font-weight:bold;}QToolButton{border:0px;}");

    mp_btn_min = new QToolButton( this );
    mp_btn_close = new QToolButton( this );

    mp_btn_min->setFocusPolicy( Qt::NoFocus );
    mp_btn_close->setFocusPolicy( Qt::NoFocus );

    set_btn_icon( mp_btn_min, eBtnStateDefault );
    set_btn_icon( mp_btn_close, eBtnStateDefault );

    mp_btn_min->installEventFilter( this );
    mp_btn_close->installEventFilter( this );
    connect( mp_btn_min, SIGNAL( clicked() ), parent, SLOT( showMinimized() ) );
    connect( mp_btn_close, SIGNAL( clicked() ), parent, SLOT( slot_exit() ) );
}

void TitleBar::set_btn_icon( QToolButton *p_btn, BtnMoustState state)
{
    if( state == eBtnStateNone ) { return; }

    QString str_image_path = get_btn_image_path( p_btn );

    QPixmap pixmap( str_image_path );\
    int n_pix_width = pixmap.width();
    int n_pix_height = pixmap.height();

    p_btn->setIcon( pixmap.copy(( n_pix_width/4 ) * ( state-1 ), 0, n_pix_width/4, n_pix_height ) );
    p_btn->setIconSize( QSize( n_pix_width/4, n_pix_height ) );
}

QString TitleBar::get_btn_image_path( QToolButton *p_btn )
{
    if ( mp_btn_min == p_btn ) {
        return ":image/image/caption_min.bmp";
    } else if ( mp_btn_close == p_btn ) {
        return ":image/image/caption_close.bmp";
    }

    return "";
}

void TitleBar::slot_close_window()
{
    emit sig_close_window();
}

void TitleBar::resizeEvent( QResizeEvent *event )
{
    mp_btn_close->move( width()-BUTTON_WIDTH-TITLE_SPACE, 0 );
    mp_btn_min->move( width()-BUTTON_WIDTH*2-TITLE_SPACE-2, 0 );
}

void TitleBar::paintEvent( QPaintEvent *event )
{
    QPainter draw( this );

    draw.setFont( m_font_title );
    draw.drawText( QRect( 0, 0, width(), height() ), m_str_title  );
    draw.drawPixmap( 0, 0, m_pixmap_logo.width(), height(), m_pixmap_logo);
    draw.drawPixmap( m_pixmap_logo.width(), 0, width()-m_pixmap_logo.width(), height(), m_pixmap_backgroud);

    draw.end();
}

bool TitleBar::eventFilter( QObject *obj, QEvent *event )
{
    if( mp_btn_min==obj || mp_btn_close==obj ){
        BtnMoustState e_state = eBtnStateNone;

        switch( event->type() ) {
        case QEvent::Enter: e_state = eBtnStateHover; break;
        case QEvent::Leave: e_state = eBtnStateDefault; break;
        case QEvent::MouseButtonPress:
            if ( ((QMouseEvent*)(event))->button()== Qt::LeftButton )
                e_state = eBtnStatePress; break;
        }

        set_btn_icon( (QToolButton *)obj, e_state );
        return false;
    }

    return QWidget::eventFilter( obj, event );
}

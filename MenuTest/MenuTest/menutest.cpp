#include "menutest.h"
#include "ui_menutest.h"

MenuTest::MenuTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MenuTest)
    ,mp_menu( NULL )
    ,mp_action_analyse_widget( NULL )
    ,mb_menu_status( false )
{
    ui->setupUi(this);
    ui->centralWidget->setMouseTracking( true );
    setMouseTracking( true );
    //右键菜单
    mp_menu = new QMenu( this );
    mp_action_analyse_widget = mp_menu->addAction( "分析窗口" );
    ui->id_frame->setContextMenuPolicy( Qt::CustomContextMenu );
    connect( ui->id_frame, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( show_contextmenu( const QPoint& ) ) );

}

MenuTest::~MenuTest()
{
    delete ui;
}

void MenuTest::show_contextmenu( const QPoint& pos )
{
    //QAction *p_action_analyse_back = mp_menu->addAction( "回放窗口" );
    if( mb_menu_status ) {
        mb_menu_status = false;
        mp_action_analyse_widget->setCheckable( false );
        mp_action_analyse_widget->setChecked( false );
    } else {
        mb_menu_status = true;
        //下面两个语句可以使得右击菜单栏显示 钩
        mp_action_analyse_widget->setCheckable( true );
        mp_action_analyse_widget->setChecked( true );
    }
    connect( mp_action_analyse_widget, SIGNAL( triggered( bool ) ), this, SLOT( slot_display_analyse_interface( bool ) ) );
    //connect( p_action_analyse_back, SIGNAL( triggered( bool ) ), this, SLOT( slot_display_back_interface( bool ) ) );
    mp_menu->exec( QCursor::pos() );//在当前鼠标位置显示
}

void MenuTest::slot_display_analyse_interface( bool b_menu )
{
    /*if( mb_menu_status ) {
        mb_menu_status = false;
        mp_action_analyse_widget->setCheckable( false );
        mp_action_analyse_widget->setChecked( false );
    } else {
        mb_menu_status = true;
        //下面两个语句可以使得右击菜单栏显示 钩
        mp_action_analyse_widget->setCheckable( true );
        mp_action_analyse_widget->setChecked( true );
    }*/




}

void MenuTest::slot_display_back_interface( bool b_menu )
{

}

void MenuTest::slot_display_analyse_change()
{
    //mp_action_analyse_widget->setCheckable( false );
    //mp_action_analyse_widget->setChecked( false );
    //mp_action_analyse_widget->setEnabled( false );
}

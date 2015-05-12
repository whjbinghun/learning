#include "wndloadpointpicture.h"
#include "ui_wndloadpointpicture.h"

WndLoadPointPicture::WndLoadPointPicture(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WndLoadPointPicture)
{
    ui->setupUi(this);

    setWindowTitle( "红外可见光" );
    Qt::WindowFlags flags = Qt::Dialog;
//    flags |= Qt::WindowMinimizeButtonHint;
//    flags |= Qt::WindowMaximizeButtonHint;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags( flags );

    init_palette();

}

WndLoadPointPicture::~WndLoadPointPicture()
{
    delete ui;
}

void WndLoadPointPicture::keyPressEvent( QKeyEvent *event )
{
    if( event->key() == Qt::Key_Control ) {
        ui->id_alarm_widget_ir->set_mouse_press_status( true );
    }

    //QWidget::keyPressEvent( event );
}

void WndLoadPointPicture::keyReleaseEvent( QKeyEvent *event )
{
    if( event->key() == Qt::Key_Control ) {
        ui->id_alarm_widget_ir->clear_ana_press_status();
    }
    //QWidget::keyReleaseEvent( event );
}

void WndLoadPointPicture::set_caption( const QString &str_caption )
{
    setWindowTitle( str_caption );
}

void WndLoadPointPicture::open_ir_vi_image( QString str_ir_path, QString str_vi_path )
{
    bool b_open_image = false;
    if ( !str_ir_path.isEmpty() )
    {
        b_open_image = ui->id_alarm_widget_ir->open_ir_image( str_ir_path );
        if( b_open_image ) {
           update_palette();
        }
    }

    if ( b_open_image ){
        ui->id_alarm_widget_palette->init_palette( clrIron, true, 10.0, 50.0, 24.3, 35.4 );
    }
    ui->id_alarm_widget_vi->set_image_path( str_vi_path );
}

void WndLoadPointPicture::delete_all_ana()
{
    ui->id_alarm_widget_ir->delete_all_ana();
}

void WndLoadPointPicture::init_palette()
{
    ui->id_alarm_widget_palette->setMouseTracking( true );
    //mp_wnd_palette为垂直
    ui->id_alarm_widget_palette->set_hor_ver( false );
    ui->id_alarm_widget_palette->set_hor_ver_image( false );

    ui->id_alarm_widget_palette->init_palette( clrIron, true, 10.0, 50.0, 24.3, 35.4 );

    connect( ui->id_alarm_widget_palette, SIGNAL( sig_set_adjust_clr( int ) ), this, SLOT( slot_palette_change_clr( int ) ) );
    connect( ui->id_alarm_widget_palette, SIGNAL( sig_set_adjust_auto() ), this, SLOT( slot_palette_auto()) );
    connect( ui->id_alarm_widget_palette, SIGNAL( sig_set_adjust_range( float,float ) ), this, SLOT( slot_palette_change_range( float,float ) ) );
}



//设置色标颜色
void WndLoadPointPicture::slot_palette_change_clr( int n_index )
{
    bool b_color_set = ui->id_alarm_widget_ir->set_adjust_color( (ColorIndex)n_index );
    if( b_color_set ) {
        ui->id_alarm_widget_ir->update_ir_image();//重新加载图片
        ui->id_alarm_widget_palette->set_color_index( (ColorIndex)n_index );
    }
}

//设置调色板自动
void WndLoadPointPicture::slot_palette_auto()
{
    float f_min_range;
    float f_max_range;
    //自动
    bool b_adjust_auto = ui->id_alarm_widget_ir->set_adjust_type( AdjustAuto );
    if( b_adjust_auto ) {
        ui->id_alarm_widget_ir->update_ir_image();//重新加载图片
        ui->id_alarm_widget_ir->get_adjust_temp( f_min_range, f_max_range );
        ui->id_alarm_widget_palette->set_adjust_auto( true );
        ui->id_alarm_widget_palette->update();
        ui->id_alarm_widget_palette->set_level_span( f_min_range, f_max_range );
    }
}

//修改色标范围
void WndLoadPointPicture::slot_palette_change_range( float f_level, float f_level_span )
{
    YFAdjustType adjust_type;
    ui->id_alarm_widget_ir->get_adjust_type( adjust_type );
    if( adjust_type != AdjustManual ) {
        bool b_adjust_type = ui->id_alarm_widget_ir->set_adjust_type( AdjustManual );
        if( !b_adjust_type ) return ;
    }

    bool b_adjust_range = ui->id_alarm_widget_ir->set_adjust_temp( f_level, f_level_span );
    if( b_adjust_range ) {
        ui->id_alarm_widget_ir->set_adjust_temp( f_level, f_level_span );
        ui->id_alarm_widget_ir->update_ir_image();//重新加载图片
    }
}

//更新调色板
void WndLoadPointPicture::update_palette()
{
    float f_level, f_level_span;
    ui->id_alarm_widget_ir->get_adjust_min_temp( f_level );
    ui->id_alarm_widget_ir->get_adjust_max_temp( f_level_span );
    YFAdjustType adjust_type;
    ui->id_alarm_widget_ir->get_adjust_type( adjust_type );
    ColorIndex color_index;
    ui->id_alarm_widget_ir->get_adjust_color( color_index );

    float f_min = f_level - (f_level_span-f_level)/2;
    float f_max = f_level_span + (f_level_span-f_level)/2;
    ui->id_alarm_widget_palette->init_palette( color_index, adjust_type == AdjustAuto, f_min, f_max, f_level, f_level_span );
   // set_palette_enable( true );
}

//设置调色板状态
void WndLoadPointPicture::set_palette_enable( bool b_enable )
{
    ui->id_alarm_widget_palette->setEnabled( b_enable );
}

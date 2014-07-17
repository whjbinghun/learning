#include "propertybrowsertest.h"
#include "ui_propertybrowsertest.h"
#include <QTextCodec>

ProPertyBrowserTest::ProPertyBrowserTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProPertyBrowserTest)
    ,mp_variant_manager( NULL )
    ,mp_bool_manager( NULL )
    ,mp_int_manager( NULL )
    ,mp_color_manager( NULL )
    ,mp_enum_manager( NULL )
    ,mp_variant_factory( NULL )
    ,mp_variant_editor( NULL )
    ,mp_btn_template_data( NULL )
    ,mb_template_auto_status( false )
    ,mp_min_template( NULL )
    ,mp_max_template( NULL )
    ,mp_item_templdate( NULL )
    ,mp_item_ir_color( NULL )
    ,mp_item_steup( NULL )
    ,mp_color_factory( NULL )
{
    ui->setupUi(this);
    mp_btn_template_data = new QPushButton( this );
    init_property_browser();
}

ProPertyBrowserTest::~ProPertyBrowserTest()
{
    delete mp_variant_manager;
    delete mp_variant_factory;
    delete mp_variant_editor;

    delete ui;
}

void ProPertyBrowserTest::init_property_browser()
{
    QTextCodec::setCodecForLocale( QTextCodec::codecForName( "UTF-8" ) );

    mp_variant_manager = new QtVariantPropertyManager( this  );
    mp_enum_manager = new QtEnumPropertyManager( this );
    mp_bool_manager = new QtBoolPropertyManager( this );
    mp_int_manager = new QtIntPropertyManager( this );
    mp_color_manager = new QtColorPropertyManager( this );


    int i = 0;
    //addProperty 加载属性
    QtProperty *top_item = mp_variant_manager->addProperty( QtVariantPropertyManager::groupTypeId(), "分析设置" );//加载父节点
    //第二个结点
    QtProperty *two_item = mp_variant_manager->addProperty( QtVariantPropertyManager::groupTypeId(), "sn" );//加载父节点

    mp_item_templdate = mp_enum_manager->addProperty("温度范围");
    QStringList enumNames;
    enumNames << "自动" << "手动";
    mp_enum_manager->setEnumNames( mp_item_templdate, enumNames );
    top_item->addSubProperty( mp_item_templdate );

    mp_max_template = mp_int_manager->addProperty( "最大温度" );
    mp_int_manager->setRange( mp_max_template, -100, 100);//设置最大最小值
    top_item->addSubProperty( mp_max_template );
    mp_min_template = mp_int_manager->addProperty( "最小温度" );
    mp_int_manager->setRange( mp_min_template, -100, 100);
    top_item->addSubProperty( mp_min_template );

    //温度要设置范围
    if( mp_item_templdate->valueText() == "自动" ) {
       mp_max_template->setEnabled( false );
       mp_min_template->setEnabled( false );
    } else {
        mp_max_template->setEnabled( true );
        mp_min_template->setEnabled( true );
    }

    mp_item_ir_color = mp_color_manager->addProperty( "颜色" );
    mp_color_manager->setValue( mp_item_ir_color, 255 );
    two_item->addSubProperty( mp_item_ir_color );

    mp_item_steup = mp_bool_manager->addProperty( "启动" );
    two_item->addSubProperty( mp_item_steup );

    mp_variant_factory= new QtVariantEditorFactory( this );
    QtEnumEditorFactory *comboBoxFactory = new QtEnumEditorFactory( this );
    mp_color_factory = new QtColorEditorFactory( this );
    QtCheckBoxFactory *checkBoxFactory = new QtCheckBoxFactory( this );
    QtSpinBoxFactory *spinBoxFactory = new QtSpinBoxFactory( this );

    //树属性浏览器
    mp_variant_editor = new QtTreePropertyBrowser( this );
    mp_variant_editor->setFactoryForManager( mp_variant_manager, mp_variant_factory );
    mp_variant_editor->setFactoryForManager( mp_enum_manager, comboBoxFactory );
    mp_variant_editor->setFactoryForManager( mp_bool_manager, checkBoxFactory );
    mp_variant_editor->setFactoryForManager( mp_int_manager, spinBoxFactory );
    mp_variant_editor->setFactoryForManager( mp_color_manager, mp_color_factory );
    mp_variant_editor->addProperty( top_item );
    mp_variant_editor->addProperty( two_item );
    mp_variant_editor->setPropertiesWithoutValueMarked( true );
    mp_variant_editor->setRootIsDecorated( false );

    connect( mp_enum_manager, SIGNAL( valueChanged( QtProperty *, int ) ),
                this, SLOT( slot_change_template_value( QtProperty *, int ) ) );
    connect( mp_bool_manager, SIGNAL( valueChanged( QtProperty*, bool ) ),
             this, SLOT( slot_bool_changed( QtProperty *, bool ) ) );
    connect( mp_int_manager, SIGNAL( valueChanged(QtProperty*,int) ),
             this, SLOT( slot_change_template(QtProperty*,int)));
    connect( mp_color_manager, SIGNAL(valueChanged(QtProperty*,QColor)),
             this, SLOT( slot_change_color(QtProperty*,QColor)) );
    //connect( mp_color_manager, SIGNAL( valueChanged( QtProperty*, QColor ) ), mp_color_factory, SLOT( slotPropertyChanged( QtProperty*, QColor ) ) );

    mp_variant_editor->show();
}

void ProPertyBrowserTest::resizeEvent( QResizeEvent *event )
{
    int n_width = ui->centralWidget->width();
    int n_height = ui->centralWidget->height();
    mp_btn_template_data->move( 0,0 );
    mp_btn_template_data->resize( n_width, 40 );

    mp_variant_editor->move( 0, 100 );
    mp_variant_editor->resize( n_width, n_height-100 );
}

void ProPertyBrowserTest::slot_change_template_value( QtProperty *property,
                                                      int value )
{
    //如果选择自动，则最大温度和最小温度将被隐藏
    if( property->valueText() == "自动" ) {
       mp_max_template->setEnabled( false );
       mp_min_template->setEnabled( false );
    } else {
        mp_max_template->setEnabled( true );
        mp_min_template->setEnabled( true );
    }

}

void ProPertyBrowserTest::slot_bool_changed( QtProperty *property, bool b_status )
{
    bool b_steup_status = false ;
    if( property == mp_item_steup ) {
        b_steup_status = b_status;
    }
}

void ProPertyBrowserTest::slot_change_color( QtProperty *property, const QColor &val )
{
    int n_green = val.green();
    int n_red = val.red();
    int n_blue = val.blue();
}

void ProPertyBrowserTest::slot_change_template( QtProperty *property, int val )
{
    int n_value;
    if( property == mp_max_template ){
        n_value = val;
    } else if( property == mp_min_template ) {
        n_value = val;
    }
}

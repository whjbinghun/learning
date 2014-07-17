#include "propertybrowsera.h"
#include <QTextCodec>
#include <QDebug>
#include "propertydockwidget.h"

PropertyBrowserA::PropertyBrowserA(QWidget *parent) :
    QWidget(parent)
    ,mp_variant_manager( NULL )
    ,mp_bool_manager( NULL )
    ,mp_int_manager( NULL )
    ,mp_color_manager( NULL )
    ,mp_enum_manager( NULL )
    ,mp_variant_factory( NULL )
    ,mp_com_box_factory( NULL )
    ,mp_color_factory( NULL )
    ,mp_check_box_factory( NULL )
    ,mp_spin_box_factory( NULL )
    ,mp_variant_editor( NULL )
    ,mb_template_auto_status( false )
    ,mp_min_template( NULL )
    ,mp_max_template( NULL )
    ,mp_item_templdate( NULL )
    ,mp_item_ir_color( NULL )
    ,mp_item_steup( NULL )
    ,mp_dock_widget( (PropertyDockWidget*) parent )
{
    init_property_browser();
}

PropertyBrowserA::~PropertyBrowserA()
{
    delete mp_variant_manager;
    delete mp_enum_manager;
    delete mp_bool_manager;
    delete mp_int_manager;
    delete mp_color_manager;
    delete mp_variant_factory;
    delete mp_com_box_factory;
    delete mp_color_factory;
    delete mp_check_box_factory;
    delete mp_spin_box_factory;

    delete mp_variant_editor;
}


void PropertyBrowserA::init_property_browser()
{
    QTextCodec::setCodecForLocale( QTextCodec::codecForName( "UTF-8" ) );

    mp_variant_manager = new QtVariantPropertyManager( this  );

    mp_enum_manager = new QtEnumPropertyManager( this );
    mp_bool_manager = new QtBoolPropertyManager( this );
    mp_int_manager = new QtIntPropertyManager( this );
    mp_color_manager = new QtColorPropertyManager( this );

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
    mp_com_box_factory = new QtEnumEditorFactory( this );
    mp_color_factory = new QtColorEditorFactory( this );
    mp_check_box_factory = new QtCheckBoxFactory( this );
    mp_spin_box_factory = new QtSpinBoxFactory( this );

    //树属性浏览器
    mp_variant_editor = new QtTreePropertyBrowser( mp_dock_widget );
    mp_variant_editor->setFactoryForManager( mp_variant_manager, mp_variant_factory );
    mp_variant_editor->setFactoryForManager( mp_enum_manager, mp_com_box_factory );
    mp_variant_editor->setFactoryForManager( mp_bool_manager, mp_check_box_factory );
    mp_variant_editor->setFactoryForManager( mp_int_manager, mp_spin_box_factory );
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

void PropertyBrowserA::resizeEvent( QResizeEvent *event )
{
    int n_height = 20;

    mp_variant_editor->move( 0, n_height );
    mp_variant_editor->resize( width(), height()-n_height*3 );
    qDebug()<<"PropertyBrowserA::resizeEvent"<<width()<<height();
}

QtTreePropertyBrowser *PropertyBrowserA::get_tree_property_browser()
{
    return mp_variant_editor;
}

void PropertyBrowserA::slot_change_template_value( QtProperty *property, int value )
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

void PropertyBrowserA::slot_bool_changed( QtProperty *property, bool b_status )
{
    bool b_steup_status = false ;
    if( property == mp_item_steup ) {
        b_steup_status = b_status;
    }
}

void PropertyBrowserA::slot_change_color( QtProperty *property, const QColor &val )
{
    int n_green = val.green();
    int n_red = val.red();
    int n_blue = val.blue();
    qDebug()<<"PropertyBrowserA::slot_change_color"<<n_green<<n_red<<n_blue;
}

void PropertyBrowserA::slot_change_template( QtProperty *property, int val )
{
    int n_value;
    if( property == mp_max_template ){
        n_value = val;
    } else if( property == mp_min_template ) {
        n_value = val;
    }
}



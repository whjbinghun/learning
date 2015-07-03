#include "treewidgettest.h"
#include "ui_treewidgettest.h"

TreeWidgetTest::TreeWidgetTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TreeWidgetTest)
{
    ui->setupUi(this);
    //ui->id_treewidget_matrix->setItemWidget();

    hLayout = new QHBoxLayout(this);
    treeWidget = new QTreeWidget(this);
    treeWidget->setColumnCount( 1 );

    QTreeWidgetItem *top = new QTreeWidgetItem;
    top->setText( 0, "" );
    treeWidget->addTopLevelItem( top );

    QCheckBox *p_box = new QCheckBox;
    p_box->setText( "top" );
    p_box->setChecked( true );
    treeWidget->setItemWidget( top, 0, p_box );



    QCheckBox *ckBox = new QCheckBox;
    ckBox->setText( "title" );
    ckBox->setChecked( true );

    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText( 0, "" );
    top->addChild( item );
    treeWidget->setItemWidget( item, 0, ckBox );                //设置ckBox为item的第二列

    QCheckBox *cpy = (QCheckBox*)treeWidget->itemWidget(item,0);
    //QMessageBox::information(this,"title",cpy->text());     //获取item中Checkbox的文本值

    hLayout->addWidget( treeWidget );
    this->setLayout( hLayout );

    //treeWidget->removeItemWidget( item, 0 );
    //treeWidget->removeItemWidget( top , 0 );
    treeWidget->clear();
}

TreeWidgetTest::~TreeWidgetTest()
{
    delete ui;
}

#include "CategoryWidget.h"
#include "ui_CategoryWidget.h"

CategoryWidget::CategoryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CategoryWidget)
{
    ui->setupUi(this);

    initCtrl();
}

CategoryWidget::~CategoryWidget()
{
    delete ui;
}

void CategoryWidget::initCtrl()
{
    CategoryInfo stCategoryInfo;
    stCategoryInfo.nId = 1;
    stCategoryInfo.sKeyId = 1;
    stCategoryInfo.sName = "苹果";
    stCategoryInfo.bCustom = false;
    m_listCategoryInfo.append( stCategoryInfo );

    stCategoryInfo.nId = 2;
    stCategoryInfo.sKeyId = 2;
    stCategoryInfo.sName = "香蕉";
    m_listCategoryInfo.append( stCategoryInfo );

    stCategoryInfo.nId = 3;
    stCategoryInfo.sKeyId = 3;
    stCategoryInfo.sName = "芒果";
    m_listCategoryInfo.append( stCategoryInfo );

    stCategoryInfo.nId = 4;
    stCategoryInfo.sKeyId = 4;
    stCategoryInfo.sName = "西瓜";
    m_listCategoryInfo.append( stCategoryInfo );

    stCategoryInfo.nId = 5;
    stCategoryInfo.sKeyId = 5;
    stCategoryInfo.sName = "哈密瓜";
    m_listCategoryInfo.append( stCategoryInfo );

    stCategoryInfo.nId = 6;
    stCategoryInfo.sKeyId = 6;
    stCategoryInfo.sName = "樱桃";
    m_listCategoryInfo.append( stCategoryInfo );

    QList<CategoryInfo>::iterator it = m_listCategoryInfo.begin();
    for( ; it != m_listCategoryInfo.end(); ++it ) {
        ui->categoryListView->appendItem( *it );
    }
}

void CategoryWidget::initConnect()
{

}

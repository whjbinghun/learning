#include "CategoryListView.h"
#include <QDebug>

static const int c_nItemHeight = 51;
static const int c_nItemWidth = 79;
static int nButtonLeftXPos = 0;
static int nButtonRightXPos = 0;
static int nButtonTopYPos = 0;
static int nButtonBottomYPos = 0;

enum CategorySellItemROLE
{
    e_CategorySellItemInfo = Qt::UserRole + 1,
    e_CategorySellCustomeItem
};


CategoryDelegate::CategoryDelegate(QWidget *parent)
    : QStyledItemDelegate( parent )
{

}

void CategoryDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    QVariant vItemInfo = index.data( e_CategorySellItemInfo );
    if( vItemInfo.canConvert<CategoryInfo>() ) {
        CategoryInfo stCategory = vItemInfo.value<CategoryInfo>();

        paintCategoryBtn( painter, option, stCategory );
    }

    painter->restore();
}

void CategoryDelegate::paintCategoryBtn( QPainter *painter, const QStyleOptionViewItem &option, CategoryInfo &stCategory ) const
{
    int xPos = option.rect.x();
    int yPos = option.rect.y();
//    int xWidth = option.rect.width();//option.rect表示是当前所画图元的区域
//    int yHeight = option.rect.height();


    static const int c_nLeftMargin = 0;
    const int c_nFirstStartX = xPos + c_nLeftMargin;
    static const int c_nFirstColWidth = c_nItemWidth;
    QFont font;
    font.setPixelSize(12);
    font.setFamily("微软雅黑");
    painter->setFont(font);
    QPen pen;
    pen.setColor( "#ffffff");
    painter->setPen( pen );

    bool bSelected = false;
    if( option.state & QStyle::State_Selected ) {
        bSelected = true;
    }
    drawCategoryImage( painter, option.rect, bSelected );
    painter->drawText(option.rect, Qt::AlignCenter, stCategory.sName );

    nButtonLeftXPos = c_nFirstStartX;
    nButtonRightXPos = nButtonLeftXPos + c_nFirstColWidth;
    nButtonTopYPos = yPos;
    nButtonBottomYPos = nButtonTopYPos + c_nItemHeight;

//    qDebug()<<"CategoryDelegate::paintCategoryBtn"<<stCategory.nId<<bSelected;
}

void CategoryDelegate::drawCategoryImage( QPainter *painter, QRect textRect, bool bSelected ) const
{
    if( bSelected ) {
        painter->drawPixmap( textRect, QPixmap(":/saleImage/goods_category_btn_press.png" )  );
    } else {
        painter->drawPixmap( textRect, QPixmap(":/saleImage/goods_category_btn_normal.png" )  );
    }
}

CategoryListView::CategoryListView(QWidget *parent)
    : QListView(parent)
    , m_pStandardItemModel(NULL)
    , m_nItemHeight(51)
    , m_nItemColumn( 5 )
    , m_nCurCategoryId( -1 )
{
    init();
}

CategoryListView::~CategoryListView()
{

}

void CategoryListView::appendItem(const CategoryInfo &stCategoryInfo )
{
    if (m_pStandardItemModel == NULL)
    {
        return;
    }

    QStandardItem* pItem = NULL;
    if (!m_ItemKeyIdMap.contains(stCategoryInfo.nId))
    {
        pItem = new QStandardItem;
        m_ItemKeyIdMap.insert(stCategoryInfo.nId, pItem);
        m_pStandardItemModel->appendRow(pItem);
        pItem->setSizeHint(QSize(c_nItemWidth, m_nItemHeight));//设置图元大小
    }
    else
    {
        pItem = m_ItemKeyIdMap.value(stCategoryInfo.nId);
    }
    QVariant variant;
    variant.setValue(stCategoryInfo);
    pItem->setData(variant, e_CategorySellItemInfo );

    if( m_nCurCategoryId == -1 ) {//初始化时默认选中第一个
        m_nCurCategoryId = stCategoryInfo.nId;
    }

    if ( m_nCurCategoryId == stCategoryInfo.nId)
    {
        this->setCurrentIndex( pItem->index() );
    }

}

void CategoryListView::clearItems()
{
    m_pStandardItemModel->clear();
    m_ItemKeyIdMap.clear();
}

void CategoryListView::mousePressEvent(QMouseEvent *event)
{
    //获取选中状态
    QModelIndex index = indexAt(event->pos());
    CategoryInfo stCategory;
    QVariant v_itemInfo = index.data( e_CategorySellItemInfo );
    if (!v_itemInfo.canConvert<CategoryInfo>())//可以转换
    {
        return;
    }
    stCategory = v_itemInfo.value<CategoryInfo>();
    m_stCurCategory = stCategory;

    if( event->x()>=nButtonLeftXPos && event->x() <= nButtonRightXPos \
            && event->y() >= nButtonTopYPos && event->y() <= nButtonBottomYPos ) {
        qDebug()<<"CategoryListView::mousePressEvent"<<m_nCurCategoryId<<stCategory.nId;
        if( m_nCurCategoryId != stCategory.nId ) {
            m_nCurCategoryId = stCategory.nId;
            this->setCurrentIndex( index );//使用当前索引，选中图元
            emit sig_categoryBtnClicked( stCategory );
        }
    }

    QListView::mousePressEvent( event );
}

void CategoryListView::init()
{
    //拥有下面三行代码，listView可以一行显示多列；如果没有下面三行代码，listView一行只能显示为一列
    setViewMode(IconMode);
    setMovement(Static);
    setSpacing(3);

    //没有滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setItemDelegateForColumn( 0, new CategoryDelegate(this));//设置图元代理forColumn
    setEditTriggers(QAbstractItemView::NoEditTriggers); // 只读
    setSelectionMode( SingleSelection );//单选
    m_pStandardItemModel = new QStandardItemModel(this);
    this->setModel(m_pStandardItemModel);
}


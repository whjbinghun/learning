#ifndef CATEGORYLISTVIEW_H
#define CATEGORYLISTVIEW_H

#include <QListView>
#include <QStyledItemDelegate>
#include <QStandardItemModel>
#include "structData.h"
#include <QMouseEvent>
#include <QPainter>


class CategoryDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CategoryDelegate(QWidget* parent = NULL);
    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
private:
    void paintCategoryBtn( QPainter *painter, const QStyleOptionViewItem &option, CategoryInfo &stCategory ) const;
    void drawCategoryImage( QPainter *painter, QRect textRect, bool bSelected ) const;
};

class CategoryListView : public QListView
{
    Q_OBJECT
public:
    explicit CategoryListView(QWidget *parent = 0);
    ~CategoryListView();

    void appendItem(const CategoryInfo& stCategoryInfo);
    void clearItems();

protected:
    void mousePressEvent(QMouseEvent *event);
signals:
    void sig_categoryBtnClicked( CategoryInfo stCategory );
public slots:

private:
    void init();
private:
    QStandardItemModel* m_pStandardItemModel;
    QMap<int, QStandardItem*> m_ItemKeyIdMap;//key：分类id
    int m_nItemHeight;
    int m_nItemColumn;
    CategoryInfo m_stCurCategory;

    int m_nCurCategoryId;
};

#endif // CATEGORYLISTVIEW_H

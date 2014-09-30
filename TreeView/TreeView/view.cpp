#include "view.h"
#include <QTextCodec>
#include <QModelIndex>
#include <QDebug>
View::View( QWidget *parent ) : QTreeView()
{
    QTextCodec::setCodecForLocale( QTextCodec::codecForName( "GBK" ) );
    model = new QStandardItemModel( 4, 2 );
    model->setHeaderData( 0, Qt::Horizontal, tr( "Service" ) );
    model->setHeaderData( 1, Qt::Horizontal, tr( "Details" ) );
    QStandardItem *item1 = new QStandardItem( "avahi-daemon" );
    item1->setIcon( QIcon( "logo.png" ) );
    QStandardItem *item2 = new QStandardItem( "bluetooth" );
    item2->setIcon( QIcon( "logo.png" ) );
    QStandardItem *item3 = new QStandardItem( "crond" );
    item3->setIcon( QIcon( "logo.png" ) );
    QStandardItem *item4 = new QStandardItem( "cups" );
    item4->setIcon( QIcon( "logo.png" ) );
    model->setItem( 0, 0, item1 );
    model->setItem( 1, 0, item2 );
    model->setItem( 2, 0, item3 );
    model->setItem( 3, 0, item4 );
    QStandardItem *item5 = new QStandardItem( "fifth" );
    item4->appendRow( item5 );//把item5结点加入到item4中
    QStandardItem *item6 = new QStandardItem( "6" );
    //item4->appendRow( item6 );
    model->setItem( 0, 1, item6 );

    QModelIndex model_index;
    //向item1中加入4个结点
    for ( int i = 0; i < 4; ++i ) {
        model_index = model->index( 0, 0, model_index );
        model->insertRows( 0, 1, model_index );
        model->insertColumns( 0, 1, model_index );
        QModelIndex index = model->index( 0, 0, model_index );
        model->setData( index, i );
    }
    this->setModel( model );
}
QList<QStandardItem*> View::returnTheItems()
{
    return model->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive );
}
void View::iterateOverItems()
{
    QList<QStandardItem*> list = returnTheItems();

    foreach ( QStandardItem* item, list ) {
        qDebug() << item->text();
    }
}
void View::mouseDoubleClickEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton ) {
        QModelIndex index0 = currentIndex();
        qDebug() << index0.data().toString();
    }
}

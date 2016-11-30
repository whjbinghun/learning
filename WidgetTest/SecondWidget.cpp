#include "SecondWidget.h"
#include <QLineEdit>

SecondWidget::SecondWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags( Qt::FramelessWindowHint | Qt::Tool );
    resize( 200, 200 );


    QLineEdit *p = new QLineEdit(this);
    p->move( 20, 20 );
    p->resize( 50, 20 );
}


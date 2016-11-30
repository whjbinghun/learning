#include "FirstWidget.h"

FirstWidget::FirstWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags( Qt::FramelessWindowHint | Qt::Tool );
}


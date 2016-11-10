#ifndef CATEGORYWIDGET_H
#define CATEGORYWIDGET_H

#include <QWidget>
#include "structData.h"

namespace Ui {
class CategoryWidget;
}

class CategoryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CategoryWidget(QWidget *parent = 0);
    ~CategoryWidget();
private:
    void initCtrl();
    void initConnect();
private:
    Ui::CategoryWidget *ui;

    QList<CategoryInfo> m_listCategoryInfo;
};

#endif // CATEGORYWIDGET_H

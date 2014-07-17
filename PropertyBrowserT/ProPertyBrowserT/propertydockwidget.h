#ifndef PROPERTYDOCKWIDGET_H
#define PROPERTYDOCKWIDGET_H

#include <QDockWidget>
#include <QPushButton>

#include "propertybrowsera.h"


class PropertyDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit PropertyDockWidget(QWidget *parent = 0);
    ~PropertyDockWidget();
public:
    void resizeEvent( QResizeEvent *event );
    void init_button();
signals:

public slots:

    void slot_copy_templdate_data();
    void slot_copy_diagram();
private:
    PropertyBrowserA *mp_property_browser;
    QPushButton *mp_btn_template_data;
    QPushButton *mp_btn_diagram;//曲线图
};

#endif // PROPERTYDOCKWIDGET_H

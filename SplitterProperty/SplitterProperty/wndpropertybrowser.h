#ifndef WNDPROPERTYBROWSER_H
#define WNDPROPERTYBROWSER_H

#include <QWidget>
#include "propertybrowsera.h"
#include <QPushButton>

class WndPropertyBrowser : public QWidget
{
    Q_OBJECT
public:
    explicit WndPropertyBrowser(QWidget *parent = 0);
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
    QPushButton *mp_btn_diagram;
};

#endif // WNDPROPERTYBROWSER_H

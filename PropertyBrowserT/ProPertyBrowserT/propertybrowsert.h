#ifndef PROPERTYBROWSERT_H
#define PROPERTYBROWSERT_H

#include <QMainWindow>
#include "propertybrowsera.h"
#include <QPushButton>

namespace Ui {
class PropertyBrowserT;
}

class PropertyBrowserT : public QMainWindow
{
    Q_OBJECT

public:
    explicit PropertyBrowserT(QWidget *parent = 0);
    ~PropertyBrowserT();
public:
    void resizeEvent( QResizeEvent *event );  
    void init_button();
public slots:
    void slot_copy_templdate_data( );
    void slot_copy_diagram();
private:
    Ui::PropertyBrowserT *ui;
    PropertyBrowserA *mp_property_browser;
    QPushButton *mp_btn_template_data;
    QPushButton *mp_btn_diagram;//曲线图
};

#endif // PROPERTYBROWSERT_H

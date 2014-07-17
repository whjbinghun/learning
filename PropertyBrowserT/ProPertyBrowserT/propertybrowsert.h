#ifndef PROPERTYBROWSERT_H
#define PROPERTYBROWSERT_H

#include <QMainWindow>
#include "propertydockwidget.h"

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
    void init_dock_widget();
private:
    Ui::PropertyBrowserT *ui;
    PropertyDockWidget *mp_dock_widget;
};

#endif // PROPERTYBROWSERT_H

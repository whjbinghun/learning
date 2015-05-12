#ifndef WNDPARENTCHILD_H
#define WNDPARENTCHILD_H

#include <QMainWindow>

namespace Ui {
class WndParentChild;
}

class WndParentChild : public QMainWindow
{
    Q_OBJECT

public:
    explicit WndParentChild(QWidget *parent = 0);
    ~WndParentChild();

private:
    Ui::WndParentChild *ui;
};

#endif // WNDPARENTCHILD_H

#ifndef WNDANAATTRUBITE_H
#define WNDANAATTRUBITE_H

#include <QWidget>

namespace Ui {
class WndAnaAttrubite;
}

class WndAnaAttrubite : public QWidget
{
    Q_OBJECT

public:
    explicit WndAnaAttrubite(QWidget *parent = 0);
    ~WndAnaAttrubite();

private:
    Ui::WndAnaAttrubite *ui;
};

#endif // WNDANAATTRUBITE_H

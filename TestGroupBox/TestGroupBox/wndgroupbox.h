#ifndef WNDGROUPBOX_H
#define WNDGROUPBOX_H


#include <QObject>
#include <QGroupBox>
#include <QWidget>

class WndGroupBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit WndGroupBox( QWidget *parent =0 );
    ~WndGroupBox();
};

#endif // WNDGROUPBOX_H

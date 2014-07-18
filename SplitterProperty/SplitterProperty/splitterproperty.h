#ifndef SPLITTERPROPERTY_H
#define SPLITTERPROPERTY_H

#include <QMainWindow>
#include "wndtempana.h"

namespace Ui {
class SplitterProperty;
}

class SplitterProperty : public QMainWindow
{
    Q_OBJECT

public:
    explicit SplitterProperty(QWidget *parent = 0);
    ~SplitterProperty();
public:
    void resizeEvent( QResizeEvent *event );
private:
    Ui::SplitterProperty *ui;
    WndTempAna *mp_temp_ana;
};

#endif // SPLITTERPROPERTY_H

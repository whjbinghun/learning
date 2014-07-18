#ifndef WNDTEMPANA_H
#define WNDTEMPANA_H

#include <QWidget>
#include <QSplitter>
#include "wndchart.h"
#include "wndpropertybrowser.h"

class WndTempAna : public QWidget
{
    Q_OBJECT
public:
    explicit WndTempAna(QWidget *parent = 0);
public:
    void resizeEvent( QResizeEvent *event );
    void init_splitter();
signals:

public slots:
private:
    QSplitter *mp_splitter_main;
    WndChart *mp_wnd_chart;
    WndPropertyBrowser *mp_wnd_property;
};

#endif // WNDTEMPANA_H

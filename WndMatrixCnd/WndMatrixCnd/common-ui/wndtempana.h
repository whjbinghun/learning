#ifndef WNDTEMPANA_H
#define WNDTEMPANA_H

#include <QWidget>
#include <QSplitter>
#include "wndcharttrend.h"
//#include "wndpropertybrowser.h"



class WndTempAna : public QWidget
{
    Q_OBJECT
public:
    explicit WndTempAna(QWidget *parent = 0,bool b_show_property=true);

public:
    void resizeEvent( QResizeEvent *event );

public:
    void init_splitter(bool b_show_property);

    void init_wnd_ctrl(int n_num_group, bool b_realtime , int n_index, bool b_preset_ana = false);

    int  get_index();

    bool get_is_preset_ana();

    void set_curr_preset( const int n_curr_preset_no );
    void set_preset_str_name( QString &str_preset_name );


    // WndPropertyBrowser * get_property_browser();
signals:

public slots:
private:
    QSplitter *mp_splitter_main;
    WndChartTrend *mp_wnd_chart;
    // WndPropertyBrowser *mp_wnd_property;

    bool mb_preset_ana; // 用来判断预置点与非预置点区分
};

#endif // WNDTEMPANA_H

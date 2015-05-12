#ifndef WNDANACHARTCOLOR_H
#define WNDANACHARTCOLOR_H

#include <QDialog>
#include "datamode.h"
#include <QComboBox>
#include "wndpropertybrowser.h"
#include "msanacolorsetting.h"
#include "wndcharttrend.h"

namespace Ui {
class WndAnaChartColor;
}

class WndAnaChartColor : public QDialog
{
    Q_OBJECT

public:
    explicit WndAnaChartColor( WndChartTrend *chart_trend, int n_index, QWidget* parent = 0 );
    ~WndAnaChartColor();

    void set_group_number( int n_group_number );
    void get_ana_chart_color_list( QMap< QString, QColor > & list_color );
    void set_ana_chart_color_list( QMap< QString, QColor > list_color );
private slots:
    void slot_btn_ok();
    void slot_btn_cancel();

    void on_mt_ana_chart_color_cellClicked(int row, int column);
private:
    void init_tabel_header();

    // 随机颜色
    QColor generate_rand_color();

    // 增加行数
    void add_data( int start_index, int rows );
private:
    Ui::WndAnaChartColor *ui;
    WndPropertyBrowser *mp_wnd_property;

    QString mstr_sn;

    int mn_index;
    bool mb_preset;
    QMap< QString, QColor > m_map_curr_ana_chart_color;




};

#endif // WNDANACHARTCOLOR_H

#ifndef MSANACOLORSETTING_H
#define MSANACOLORSETTING_H

#include <QApplication>
#include <QSettings>
#include <QString>
#include <QColor>

struct AnaChartColor {
    QString str_sn;
    int     ana_id;
    int     type;       // 最大、最小、平均
    QColor  color;

    AnaChartColor(){
        str_sn = "";
        ana_id = 0;
        type = 0;
        color = Qt::white;
    }
};

class MSAnaColorSetting
{
public:
    MSAnaColorSetting( );
    ~MSAnaColorSetting();

    static MSAnaColorSetting *Instance();
    static void Release();

    QStringList get_ana_color_keys( int n_group_number, int n_index );
    void        clear_ana_colors( int n_group_number, int n_index );

    void set_ana_chart_color( int n_group_number, int n_index, QString str_name, QColor color );
    QColor get_ana_chart_color( int n_group_number, int n_index,  QString str_name );
private:
    static MSAnaColorSetting* m_ana_color_setting;

    QString mstr_path;
    QSettings* m_settings;

};

#endif // MSANACOLORSETTING_H

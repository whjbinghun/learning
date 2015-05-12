#ifndef MATRIXWIDGET_H
#define MATRIXWIDGET_H

#include <QWidget>
#include <QDebug>
#include "matrixfactory.h"
#include <QMutex>
#include <QMutexLocker>
#include "datamode.h"
#include "cirpreview.h"
#include "netcommon.h"
#include "cbasemsg.h"
#include "mscclienttype.h"
#include <string>

/*
 * MatrixWidget类，预置点矩阵分析管理
 * 初始化的分析由外部设置
 * 内部新增量、修改、删除分析由自己管理
 * */

namespace Ui {
class MatrixWidget;
}

class MatrixLayout;
class QMenu;
class QAction;
class QMutex;
class MatrixWidget;


enum MatrixSelectMode_E
{
    MATRIXSELECTMODE_NULL = 0,
    MATRIXSELECTMODE_NEW,      // 是否可以画矩形框
    MATRIXSELECTMODE_SINGLE,   // press, release在同一单位
    MATRIXSELECTMODE_DRAG_DROP // press, release不在同一单位
};


// 单元格的信息
struct MatrixLayoutInfo_S
{
    int     row;
    int     col;
    int     index;
    QColor  color;
    bool    select_status;  // 选中状态1选中,0取消选中
    QRectF   rect;

    // 组号，用于分组
    int     group_index;

    bool    is_alarming;
    int     alarm_times;

    MatrixLayoutInfo_S( )
    {
        index           = -1;
        group_index     = -1;
        select_status   = 0;
        color           = Qt::white;
        is_alarming     = false;
        alarm_times     = 0;
    }

    bool is_new( )   // 是否新增允许，取决于select_status
    {
        bool ret = false;
        // 等于0，状态未选中是允许增加区域,画框
        if ( select_status == 0){
            ret = true;
        }
        return ret;
    }

};

// 矩阵分组
class MatrixGroup
{
public:
    MatrixGroup( int group_id=-1, QColor color=Qt::white, MatrixWidget* widget = 0 ):
        mb_alarming(false)
    {
        m_cell_list.clear();
        mn_group_id = group_id;
        mc_group_color = color;
        mw_matrix_widget = widget;
        me_ana_of_client = ClientNone;

        ms_name = "matrix_" + QString::number(group_id); //  get_color_to_string() +
    }
    /**
    * @brief
    * get_group_name 获取组名
    * author slf
    */
    QString & get_group_name( ){
        return ms_name;
    }
    /**
    * @brief
    * get_color_name 获取矩阵颜色名称
    * author slf
    */
    QString get_color_name(){
        QString temp_str ="未知";

        if ( mc_group_color == Qt::green ){
            temp_str = "绿色阵列";
        }else if( mc_group_color == Qt::cyan ){
            temp_str = "青色阵列";
        }else if( mc_group_color == Qt::blue ){
            temp_str = "蓝色阵列";
        }else if( mc_group_color == Qt::yellow ){
            temp_str = "黄色阵列";
        }
        return temp_str;
    }
    /**
    * @brief
    * start_alarming 开启报警
    * author slf
    */
    void start_alarming()
    {
        mb_alarming = true;
    }

    /**
    * @brief
    * stop_alarming 停止报警
    * author slf
    */
    void stop_alarming()
    {
        mb_alarming = false;
    }

    /**
    * @brief
    * is_used 是否在使用
    * author slf
    */
    bool is_used()
    {
        return m_cell_list.size() > 0;
    }
    /**
    * @brief
    * get_group_id 获取组id
    * author slf
    */
    int get_group_id()
    {
        return mn_group_id;
    }
    /**
    * @brief
    * get_group_color 获取组颜色
    * author slf
    */
    QColor get_group_color()
    {
        return mc_group_color;
    }

    /**
    * @brief
    * add_cell 增加单元格信息到组中
    * param[in] cell 单元格信息
    * author slf
    */
    bool add_cell( MatrixLayoutInfo_S cell )
    {
        bool ret = false;
        bool b_have = false;
        QMutexLocker locker(&m_cell_list_lock);
        // 存在不允许增加
        QVector<MatrixLayoutInfo_S>::iterator it = m_cell_list.begin();
        for(; it != m_cell_list.end(); it++){
            if (cell.index == it->index ){
                b_have = true;
                break;
            }
        }

        if ( !b_have ){
            m_cell_list.push_back( cell );
            ret = true;
        }
        else{
            qDebug()<<"已在存在cell info";
        }

       // qDebug()<<"add_cell group_id:"<<mn_group_id<<" size:" << m_cell_list.size();
        return ret;
    }

    //
    /**
    * @brief
    * add_cell 从组中删除单元格信息
    * param[in] cell_index 单元格索引
    * author slf
    */
    bool del_cell( int cell_index )
    {
        bool ret = false;
        QMutexLocker locker( &m_cell_list_lock );
        QVector< MatrixLayoutInfo_S >::iterator it = m_cell_list.begin();
        for( ; it != m_cell_list.end(); it++ )
        {
            if ( cell_index == it->index ){
                m_cell_list.erase( it );
                ret = true;
                break;
            }
        }
        return ret;
    }

    /**
    * @brief
    * clear
    * author slf
    */
    void clear(){
        mb_alarming = false;
        m_cell_list.clear();
    }

    /**
    * @brief
    * set_ana_of_client 设置客户端类型
    * param[in] ana_of_client 客户端类型
    * author slf
    */
    void set_ana_of_client( ClientType ana_of_client )
    {
        me_ana_of_client = ana_of_client;
    }

    /**
    * @brief
    * add_matrix_ana_to_server 增加矩阵分析到服务器
    * author slf
    */
    bool add_matrix_ana_to_server();
    /**
    * @brief
    * chg_matrix_ana_to_server 修改矩阵分析到服务器
    * author slf
    */
    bool chg_matrix_ana_to_server();
    /**
    * @brief
    * del_matrix_ana_to_server 删除矩阵分析到服务器
    * author slf
    */
    bool del_matrix_ana_to_server();

public:
    QMutex                      m_cell_list_lock;
    QVector<MatrixLayoutInfo_S> m_cell_list;
    QString                     ms_name;            // 矩阵组别名
    bool                        mb_alarming;
    ClientType              me_ana_of_client;   // 是是那一个个客户端画的分析

private:
    int             mn_group_id;
    QColor          mc_group_color;
    //  表示是否改变, 在使用string_to_matrix_list方法是不用改变状态; 当单独改变的时候， 提供方法去更改这个状态
    MatrixWidget*   mw_matrix_widget;

};


class MatrixWidget : public QWidget,public CIrPreview
{
    Q_OBJECT
    Q_ENUMS( MatrixSelectMode_E )

public:
    explicit MatrixWidget( QWidget *parent = 0 );
    ~MatrixWidget();
    /**
    * @brief
    * set_preset_params 设置组信息
    * param[in] group_number 组号
    * param[in] rail 导轨号
    * param[in] ptz 云台号
    * author slf
    */
    void set_preset_params( const int group_number, const int rail, const int ptz );
    /**
    * @brief
    * set_preset_params 设置组信息
    * param[in] type 实时或回放 1, 2
    * param[in] sn 设备sn
    * author slf
    */
    void set_sn_info( const int type, const QString sn );
    /**
    * @brief
    * get_matrix_group_list 获取矩阵组, 用于增加矩阵分析
    * author slf
    */
    QVector< MatrixGroup* > get_matrix_group_list();
    /**
    * @brief
    * string_to_matrix_list 解析字符串信息成矩阵列表
    * param[in] ana_id 分析ID
    * param[in] str_string
    * param[in] ana_of_client 客户端
    * author slf
    */
    // operator_type 0增，1改， 2删除
    bool string_to_matrix_list( int ana_id, const QString &str_string, ClientType ana_of_client );
    // 开启停止报警 alarm_times参数， 在b_alarm=true时候起作用
    /**
    * @brief
    * set_matrix_alarm 解析字符串信息成矩阵列表
    * param[in] ana_id 分析ID
    * param[in] str_string
    * param[in] ana_of_client 客户端
    * author slf
    */
    void set_matrix_alarm( const int ana_id, const bool b_alarm, const int alarm_times = 0 );
    //
    /**
    * @brief
    * get_ana_id_by_group_index 通过组找分析ID
    * param[in] group 矩阵组
    * param[out] 分析id
    * author slf
    */
    int get_ana_id_by_group_index( MatrixGroup *group );
    //
    /**
    * @brief
    * matrix_clear 清除数据
    * author slf
    */
    void matrix_clear();
    /**
    * @brief
    * matrix_clear 清除数据
    * param[in] group_id 矩阵组ID
    * author slf
    */
    void matrix_clear( int group_id );

    //
    /**
    * @brief
    * get_matrix_color_by_ana_id 通过分析ID找组颜色
    * param[in] n_ana_id 分析ID
    * param[out] out_color_name 颜色
    * author slf
    */
    void get_matrix_color_by_ana_id( const int n_ana_id, char* out_color_name );
    /**
    * @brief
    * get_ana_id_by_matrix_color 通过组颜色找分析ID
    * param[in] matrix_color 颜色
    * author slf
    */
    int get_ana_id_by_matrix_color( const QString matrix_color );
    //2015.3.4 blm add 预置点删除时清除矩阵区域
    void clear_matrix_ana_info();

signals:
    //
    /**
    * @brief
    * get_matrix_color_by_ana_id 窗口隐藏信号
    * author slf
    */
    void matrix_widget_hide();
    //
    /**
    * @brief
    * sig_add_preset_matrix_ana_to_server_success 成功增加预置点矩阵分析到服务器信号
    * param[in] ana_item 分析ID
    * param[in] rail 导轨
    * param[in] ptz 云台
    * author slf
    */
    void sig_add_preset_matrix_ana_to_server_success( AnaItem& ana_item, int rail, int ptz );

    /**
    * @brief
    * sig_chg_preset_matrix_ana_to_server_success 成功修改预置点矩阵分析到服务器信号
    * param[in] ana_item 分析ID
    * param[in] rail 导轨
    * param[in] ptz 云台
    * author slf
    */
    void sig_chg_preset_matrix_ana_to_server_success( AnaItem& ana_item, int rail, int ptz );

    /**
    * @brief
    * sig_del_preset_matrix_ana_to_server_success 成功删除预置点矩阵分析到服务器信号
    * param[in] ana_item 分析ID
    * param[in] rail 导轨
    * param[in] ptz 云台
    * author slf
    */
    void sig_del_preset_matrix_ana_to_server_success( AnaItem& ana_item, int rail, int ptz );

    //更新报警条件
    void sig_update_alarm_info();
protected:
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    // drag and drop
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    int  onResponse(CBaseMsg &reqMsg, CBaseMsg &rspMsg);

private slots:
    //
    /**
    * @brief
    * slot_style_20_20 样式 20*20
    * author slf
    */
    void slot_style_20_20();
    //
    /**
    * @brief
    * go_back_slot 返回按钮
    * author slf
    */
    void go_back_slot();
    /**
    * @brief
    * clear_act clear action
    * author slf
    */
    void clear_act();
    /**
    * @brief
    * clear_green_slot clear green
    * author slf
    */
    void clear_green_slot();
    /**
    * @brief
    * clear_yarn_slot clear yarn
    * author slf
    */
    void clear_yarn_slot();
    /**
    * @brief
    * clear_blue_slot clear blue
    * author slf
    */
    void clear_blue_slot();
    /**
    * @brief
    * clear_yellow_slot clear yellow
    * author slf
    */
    void clear_yellow_slot();
    /**
    * @brief
    * on_m_btn_matrix_back_clicked 返回按钮槽
    * author slf
    */
    void on_m_btn_matrix_back_clicked();
    void clear_all_region_slot();

private:
    //
    /**
    * @brief
    * init_menu 初始化菜单
    * author slf
    */
    void init_menu();
    //
    /**
    * @brief
    * is_valid_point 点是否有效
    * param[in] pos 点
    * author slf
    */
    bool is_valid_point ( QPoint pos);
    /**
    * @brief
    * produce_style 产生风格样式
    * param[in] rows 行数
    * param[in] cols 列数
    * param[in] w 宽度
    * param[in] h 高度
    * author slf
    */
    void produce_style ( const int rows, const int cols, const int w, const int h);
    //
    /**
    * @brief
    * get_cell_info_by_pos 通过鼠标松开所在的点判断单元格
    * param[in] pos 点
    * param[out] out_info 单元格信息
    * author slf
    */
    int  get_cell_info_by_pos ( QPoint pos, MatrixLayoutInfo_S* out_info);
    //
    /**
    * @brief
    * get_cell_info 通过行，列获取单元格
    * param[in] row 行
    * param[in] col 列
    * author slf
    */
    MatrixLayoutInfo_S get_cell_info ( int row, int col);
    //
    /**
    * @brief
    * get_cell_info_by_rect 通过矩形判断选中的单元格
    * param[in] begin_pos 起点
    * param[in] end_pos 终点
    * param[out] cell_list 单元格列表
    * author slf
    */
    bool get_cell_info_by_rect ( QPoint begin_pos, QPoint end_pos, QMap<int, MatrixLayoutInfo_S>& cell_list );
    //
    /**
    * @brief
    * cell_map_info 将选中的单元格加入组列表中
    * param[in] cell_map_info 起点
    * param[out] MatrixGroup 组
    * author slf
    */
    MatrixGroup * add_cell_to_group_map( QMap<int, MatrixLayoutInfo_S> cell_map_info, bool changed = false );
    //
    /**
    * @brief
    * add_cell_to_group_map 增加单个单元格到组列表中
    * param[in] group_index 组索引
    * param[in] cell_info 单无格信息
    * param[out] cell_info 组
    * author slf
    */
    MatrixGroup * add_cell_to_group_map( int group_index, MatrixLayoutInfo_S cell_info, bool changed = false );
    //
    /**
    * @brief
    * del_cell_from_group_map 删除从选中到未选中的状态
    * param[in] group_index 组索引
    * param[in] cell_index 单元格索引
    * author slf
    */
    MatrixGroup *del_cell_from_group_map( int group_index, int cell_index );
    //
    /**
    * @brief
    * is_exist_not_used_group 是否存在没有使用的组， 返回组
    * param[in] group_index 组索引
    * param[in] cell_index 单元格索引
    * author slf
    */
    MatrixGroup* is_exist_not_used_group(  );
    //
    /**
    * @brief
    * get_group_by_group_id 通过组号查找组
    * param[in] group_id 组索引
    * author slf
    */
    MatrixGroup* get_group_by_group_id( int group_id );
    //
    /**
    * @brief
    * get_group_by_group_name 通过组名查找组
    * param[in] group_name 组名称
    * author slf
    */
    MatrixGroup* get_group_by_group_name( const QString group_name );
    //
    /**
    * @brief
    * init_cell_group 初始化组
    * author slf
    */
    void init_cell_group(  );
    //
    /**
    * @brief
    * uninit_cell_group 反初始化组
    * author slf
    */
    void uninit_cell_group(  );
    //
    /**
    * @brief
    * get_cell_info_by_two_point 通过两个点确定一个矩形
    * param[in] start_pos 起点
    * param[in] end_pos 终点
    * author slf
    */
    MatrixLayoutInfo_S* get_cell_info_by_two_point( QPoint start_pos, QPoint end_pos );
    /**
    * @brief
    * get_cell_info_by_two_point 通过两个点确定一个矩形
    * param[in] start_pos 起点
    * param[in] end_pos 终点
    * author slf
    */
    int get_cell_info_by_two_point( QPoint start_pos, QPoint end_pos, MatrixLayoutInfo_S& cell_info  );
    /**
    * @brief
    * start_group_alarm 开启组报警
    * param[in] group 组
    * param[in] alarm_times 报警次数
    * author slf
    */
    //
    bool start_group_alarm( MatrixGroup* group , int alarm_times );
    //
    /**
    * @brief
    * start_group_alarm 停止组报警
    * param[in] group 组
    * author slf
    */
    void stop_group_alarm(  MatrixGroup* group  );
public:
    //窗口的原始size
    int mn_org_width;
    int mn_org_height;
    //当前窗口的size与原始size的比例
    float mf_sz_width;
    float mf_sz_height;
    int mn_type;
    int mn_group_number; // 组号， 与本类的组不是同个概念
    QString ms_sn;  // 设备sn
    int mn_rail;   // 导轨号
    int mn_ptz;    // 云台号
private:
    Ui::MatrixWidget *ui;
    // 单元格信息列表key是索引值， value单元格是信息
    QMap<int, MatrixLayoutInfo_S>       m_layout_info_map;
    // 单元格信息列表操作锁
    QMutex                              m_layout_info_map_lock;
    // 创建风格样式的工厂
    MatrixFactory*                      m_layout_factory;
    MatrixSelectMode_E                  m_matrix_select_mode_e;
    // 在MATRIXSELECTMODE_SINGLE模型下， press与release 是同一个单元格才会状态改变
    int mn_curr_select_index;//当前选中索引
    // 右键主菜单
    QMenu *m_menu;
    QMenu *m_style_sub_menu;
    QAction* m_style20_20_act;
    QAction* m_style18_18_act;
    QAction* m_style16_16_act;
    QAction* m_style10_10_act;
    QAction* m_style8_8_act;
    QAction* m_style6_6_act;
    QAction* m_clear_act;
    QAction* m_back_act;
    QAction* m_green_act;
    QAction* m_cyan_act;
    QAction* m_blue_act;
    QAction* m_yellow_act;
    QAction* m_clear_all_act;

    // 记录画框的开始位置
    QPoint  m_draw_start_pos;
    // 记录画框的结束位置
    QPoint  m_draw_end_pos;
    // 记录拖放的开始位置
    QPoint  m_drag_start_pos;
    /*已选择单元格分组信息, 现在支持到最多4组，每一组用不同的颜色标识
    * key组索引， value单元格MatrixLayoutInfo_S
    **/
    QMutex                                          m_cell_group_list_lock;
    QVector<MatrixGroup*>                           m_cell_group_list;
    // key指针, value分析ID, 矩阵组与分析ID的列表
    QMutex                                          m_group_ana_id_list_lock;
    QMap<MatrixGroup*, int>                         m_group_ana_id_list;

};

#endif // WIDGET_H

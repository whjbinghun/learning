#include "matrixwidget.h"
#include "ui_matrixwidget.h"
#include "ir_ana_label.h"
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QDataStream>
#include <QMimeData>
#include <QPixmap>
#include <QDrag>
#include <QMessageBox>

MatrixWidget::MatrixWidget( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::MatrixWidget )
   , mn_type(  -1  )
   , mn_group_number(  - 1 )
   , mn_rail(  -1  )
   , mn_ptz(  -1  )
   , ms_sn(  ""  )
   , m_matrix_select_mode_e(  MATRIXSELECTMODE_NULL  )
   , mn_curr_select_index(  -1  )
{
    ui->setupUi(  this  );
    setAcceptDrops(  true  );
    init_menu(  );
    init_cell_group(  );
    MatrixFactory::Instance(  );

}

MatrixWidget::~MatrixWidget(  )
{
    uninit_cell_group(  );
    MatrixFactory::Release(  );
    delete ui;
}

void MatrixWidget::set_preset_params(  const int group_number, const int rail, const int ptz  )
{
    mn_group_number = group_number;
    mn_rail         = rail;
    mn_ptz          = ptz;
}

void MatrixWidget::set_sn_info(  const int type, const QString sn  )
{
    mn_type = type;
    ms_sn = sn;
}

bool MatrixWidget::string_to_matrix_list(int ana_id, const QString &str_string, ClientType ana_of_client  )
{
    bool ret = false;

    QMap<int, MatrixLayoutInfo_S> m_temp_cell_info_list;
    // "R:x,y;x,y;-R:x,y;x,y;-"
    // 横杠分割出来的列表
    QStringList str_horizontal_bar_list = str_string.split( '-' ); // 分割横
    for(  int i=0; i<str_horizontal_bar_list.size(  ); i++  ){
//        if ( i == str_horizontal_bar_list.size() - 1 ){
//            qDebug() << "进入断点！";
//        }
        QString str_temp = str_horizontal_bar_list.at( i );
        if (  str_temp.isEmpty(  )  ){
            continue;
        }
        QStringList colon_list = str_temp.split( ":" ); // 逗号分割

        // 索引号为0是 R
        QString str_type = colon_list[0];
        if (  str_type != "R"  ) {
            break;
        }

        QString str_semicolon = colon_list[1]; // 分号串x,y;x,y;
        if (  str_semicolon.isEmpty(  )  ){
            continue;
        }

        // 通过分号分割字符串
        QStringList str_semicolon_list = str_semicolon.split( ';' );
        if ( str_semicolon_list.size() < 2 ){
            continue;
        }
        // 开始点
        QPoint start_pos( -1,-1 );
        QString str_start_pos = str_semicolon_list[0];
        // 通过逗号分割字符串
        QStringList str_start_pos_comma_list = str_start_pos.split( ',' );
        if (  str_start_pos_comma_list.size(  ) == 2  ){
            start_pos.setX( str_start_pos_comma_list[0].toInt(  ) );
            start_pos.setY( str_start_pos_comma_list[1].toInt(  ) );
        }
        // 结束点
        QPoint end_pos( -1,-1 );
        QString str_end_pos = str_semicolon_list[1];
        // 通过逗号分割字符串
        QStringList str_end_pos_comma_list = str_end_pos.split( ',' );
        if (  str_end_pos_comma_list.size(  ) == 2  ){
            end_pos.setX(  str_end_pos_comma_list[0].toInt(  )  );
            end_pos.setY(  str_end_pos_comma_list[1].toInt(  )  );
        }

        // 找到两点在单元格的内的位置
        if (  (  is_valid_point(  start_pos  )  ) && (  is_valid_point(  end_pos  )  )  ){

            // 一个分析
            // 相对比例换算
            int n_pt_start_x    = start_pos.x(  )*mf_sz_width;
            int n_pt_start_y    = start_pos.y(  )*mf_sz_height;
            int n_pt_end_x      = end_pos.x(  )*mf_sz_width;
            int n_pt_end_y      = end_pos.y(  )*mf_sz_height;
            QPoint pt_start     = QPoint(  n_pt_start_x, n_pt_start_y  );
            QPoint pt_end       = QPoint(  n_pt_end_x, n_pt_end_y  );

            MatrixLayoutInfo_S cell_info;
            int ret = get_cell_info_by_two_point(  pt_start, pt_end, cell_info  );
            if (  ret != -1  ){
                m_temp_cell_info_list[ret] =  cell_info;
            }
        }

    }

    MatrixGroup* group = is_exist_not_used_group(  );
    if (  group  ){
        group->set_ana_of_client(  ana_of_client  );

        QMap<int, MatrixLayoutInfo_S>::iterator it = m_temp_cell_info_list.begin(  );
        for(  ; it !=m_temp_cell_info_list.end(  ); it++  ){
            MatrixLayoutInfo_S info = it.value(  );
            info.select_status      = 1;
            info.color              = group->get_group_color(  );
            info.group_index        = group->get_group_id(  );
            QMap<int, MatrixLayoutInfo_S> cell_map;
            cell_map[ info.index ] = info;
            // 更新到单元格信息中
            m_layout_info_map[ info.index ] = info;

            // 更新到组中
            group->add_cell(  info  );
        }
        m_group_ana_id_list[ group ] = ana_id;
    }
    return ret;
}

void MatrixWidget::set_matrix_alarm(  const int ana_id, const bool b_alarm, const int alarm_times  )
{
    QMap<MatrixGroup*, int>::iterator it = m_group_ana_id_list.begin(  );
    for(  ; it != m_group_ana_id_list.end(  ); it++  ){
        MatrixGroup* group = it.key(  );
        int n_ana_id = it.value(  );
        if (  n_ana_id == ana_id  ){
            if (  b_alarm  ){
                start_group_alarm(  group, alarm_times  );
            }else{
                stop_group_alarm(  group  );
            }
            break;
        }
    }
    // 刷新界面
    update(  );
}

void MatrixWidget::init_menu(  )
{
    m_menu=new QMenu( this );
    if (  MSCClientType::Instance(  )->get_client_type(  ) == ClientPresetSet  ){

        m_clear_act = new QAction(  tr( "清除" ), this  );
        connect(  m_clear_act, SIGNAL(  triggered(  )  ), this, SLOT(  clear_act(  )  )  );


        m_style_sub_menu = new QMenu(  tr( "清除区域" ),this  );
        m_green_act = new QAction(  tr( "清除绿色区域" ), this  );
        m_cyan_act  = new QAction(  tr( "清除青色区域" ), this  );
        m_blue_act  = new QAction(  tr( "清除蓝色区域" ), this  );
        m_yellow_act = new QAction(  tr( "清除黄色区域" ), this  );
        m_clear_all_act = new QAction( tr("清除所有区域"), this );
        connect( m_green_act, SIGNAL(  triggered(  ) ), this, SLOT( clear_green_slot(  )  )  );
        connect( m_cyan_act, SIGNAL(  triggered(  ) ), this, SLOT( clear_yarn_slot(  ) ) );
        connect( m_blue_act, SIGNAL(  triggered(  ) ), this, SLOT( clear_blue_slot(  ) ) );
        connect( m_yellow_act, SIGNAL(  triggered(  ) ), this, SLOT( clear_yellow_slot(  ) ) );
        connect( m_clear_all_act, SIGNAL(  triggered(  ) ), this, SLOT( clear_all_region_slot(  ) ) );

        m_style_sub_menu->addAction( m_green_act );
        m_style_sub_menu->addAction( m_cyan_act );
        m_style_sub_menu->addAction(  m_blue_act );
        m_style_sub_menu->addAction(  m_yellow_act );
        m_style_sub_menu->addAction(  m_clear_all_act );
        m_clear_act->setMenu(  m_style_sub_menu );

        m_menu->addAction(  m_clear_act );
        m_menu->addSeparator(   );
    }

    m_back_act = new QAction(  tr(  "退出" ), this );
    connect(  m_back_act, SIGNAL(  triggered(   ) ), this, SLOT(  go_back_slot(   ) ) );

    m_menu->addAction(  m_back_act );

    //ui->m_btn_matrix_back->setMenu(  m_menu );
    connect(   ui->m_btn_matrix_back, SIGNAL(  clicked(   ) ), this, SLOT(   go_back_slot(   )  )  );
}

void MatrixWidget::on_m_btn_matrix_back_clicked(   )
{

}


void MatrixWidget::init_cell_group(   )
{
    MatrixGroup* group_1 = new MatrixGroup(  0, Qt::green, this );
    m_cell_group_list.push_back(  group_1 );
    m_group_ana_id_list[group_1] = -1;

    MatrixGroup* group_2 = new MatrixGroup(  1, Qt::cyan, this );
    m_cell_group_list.push_back(  group_2 );
    m_group_ana_id_list[group_2] = -1;


    MatrixGroup* group_3 = new MatrixGroup(  2, Qt::blue, this );
    m_cell_group_list.push_back(  group_3 );
    m_group_ana_id_list[group_3] = -1;


    MatrixGroup* group_4 = new MatrixGroup(  3, Qt::yellow, this );
    m_cell_group_list.push_back(  group_4 );
    m_group_ana_id_list[group_4] = -1;

}

void MatrixWidget::uninit_cell_group(   )
{
    for(   int i=0; i<m_cell_group_list.size(   ); i++ ){
        MatrixGroup* group = m_cell_group_list.at(  i );
        if (  group ){
            delete group;
            group = 0;
        }
    }
    m_cell_group_list.clear(   );
}

MatrixLayoutInfo_S* MatrixWidget::get_cell_info_by_two_point(  QPoint start_pos, QPoint end_pos )
{
    MatrixLayoutInfo_S* cell = 0;

    for(   int i=0; i<m_layout_info_map.size(   ); i++ ){
        MatrixLayoutInfo_S info = m_layout_info_map[i];

        QRect temp_rect(  start_pos, end_pos );
        QPoint center_pt = temp_rect.center(   );
        // 中心点在区域内， 整个主在区域内
        if (  info.rect.contains(  center_pt.x(   ), center_pt.y(   ) ) ){
            cell = &info;
            break;
        }

//        if (  info.rect.topLeft(   ).x(   )==start_pos.x(   ) && info.rect.topLeft(   ).y(   )==start_pos.y(   )&&
//                info.rect.bottomRight(   ).x(   )==end_pos.x(   ) && info.rect.bottomRight(   ).y(   )==end_pos.y(   ) ){
//            cell = &info;
//        }
    }
    return cell;
}

int MatrixWidget::get_cell_info_by_two_point(  QPoint start_pos, QPoint end_pos, MatrixLayoutInfo_S &cell_info )
{
    int ret = -1;

    for(   int i=0; i<m_layout_info_map.size(   ); i++ ){
        MatrixLayoutInfo_S info = m_layout_info_map[i];

        QRect temp_rect(  start_pos, end_pos );
        QPoint center_pt = temp_rect.center(   );
        // 中心点在区域内， 整个主在区域内
        if (  info.rect.contains(  center_pt.x(   ), center_pt.y(   ) ) ){
            ret = info.index;
            cell_info = info;
            break;
        }

//        if (  info.rect.topLeft(   ).x(   )==start_pos.x(   ) && info.rect.topLeft(   ).y(   )==start_pos.y(   )&&
//                info.rect.bottomRight(   ).x(   )==end_pos.x(   ) && info.rect.bottomRight(   ).y(   )==end_pos.y(   ) ){
//            cell = &info;
//        }
    }
    return ret;
}

bool MatrixWidget::start_group_alarm(  MatrixGroup *group, int alarm_times )
{
    if (  group ){
        group->start_alarming(   );
        // 同步cell_list列表
        QMutexLocker locker(  &m_layout_info_map_lock );
        QMap<int, MatrixLayoutInfo_S>::iterator it = m_layout_info_map.begin(   );
        for (  ; it != m_layout_info_map.end(   ); it++ ){
            MatrixLayoutInfo_S cell = it.value(   );
            if (  cell.group_index == group->get_group_id(   ) ){
                cell.is_alarming = true;
                cell.alarm_times = alarm_times;

                m_layout_info_map[cell.index]= cell;
            }
        }
    }
    return true;
}

void MatrixWidget::stop_group_alarm(  MatrixGroup *group )
{
    if (  group ){
        group->stop_alarming(   );

        // 同步cell_list列表
        QMutexLocker locker(  &m_layout_info_map_lock );
        QMap<int, MatrixLayoutInfo_S>::iterator it = m_layout_info_map.begin(   );
        for (  ; it != m_layout_info_map.end(   ); it++ ){
            MatrixLayoutInfo_S cell = it.value(   );
            if (  cell.group_index == group->get_group_id(   ) ){
                cell.is_alarming = false;
                cell.alarm_times = 0;
                break;
            }
        }
    }
}

int MatrixWidget::get_ana_id_by_group_index(  MatrixGroup* group )
{
    return m_group_ana_id_list[group];
}

void MatrixWidget::matrix_clear(   )
{
    {
        QMutexLocker locker(  &m_layout_info_map_lock );
        QMap<int, MatrixLayoutInfo_S>::iterator it = m_layout_info_map.begin(   );
        for (  ; it != m_layout_info_map.end(   ); it++ ){
            MatrixLayoutInfo_S cell = it.value(   );
            cell.group_index = -1;
            cell.select_status = 0;
            cell.color = Qt::white;
            cell.is_alarming = false;
            cell.alarm_times = 0;

            m_layout_info_map[it.key(   )] = cell;
        }
    }

    {
        QMutexLocker locker(  &m_cell_group_list_lock );
        QVector<MatrixGroup*>::iterator it = m_cell_group_list.begin(   );
        while (   it != m_cell_group_list.end(   )  ){
            MatrixGroup* group = *it;
            if (  group ){
                group->clear(   );
            }
            it++;
        }
    }

    update(   );
}

void MatrixWidget::matrix_clear(  int group_id )
{
    QMutexLocker locker(  &m_layout_info_map_lock );
    QMap<int, MatrixLayoutInfo_S>::iterator it = m_layout_info_map.begin(   );
    for (  ; it != m_layout_info_map.end(   ); it++ ){
        MatrixLayoutInfo_S cell = it.value(   );
        if (  group_id == cell.group_index ) {
            cell.group_index = -1;
            cell.select_status = 0;
            cell.color = Qt::white;
            cell.is_alarming = false;
            cell.alarm_times = 0;

            m_layout_info_map[it.key(   )] = cell;
        }
    }
}

void MatrixWidget::get_matrix_color_by_ana_id(  const int n_ana_id, char *out_color_name )
{
    QMutexLocker locker(  &m_group_ana_id_list_lock );
    QMap<MatrixGroup*, int>::iterator it = m_group_ana_id_list.begin(   );
    for(  ; it != m_group_ana_id_list.end(   ); it++ ){
        if (   it.value(   ) == n_ana_id  ){
            if (  out_color_name ){
                strcpy(  out_color_name, it.key(   )->get_color_name(   ).toStdString(   ).data(   ) );
                return;
            }
        }
    }
    strcpy(  out_color_name, "未知" );
}

int MatrixWidget::get_ana_id_by_matrix_color(   const QString matrix_color  )
{
    QMutexLocker locker(  &m_group_ana_id_list_lock );
    QMap<MatrixGroup*, int>::iterator it = m_group_ana_id_list.begin(   );
    for(  ; it != m_group_ana_id_list.end(   ); it++ ){
        MatrixGroup* group = it.key(   );
        if (   group  ){
            if (   matrix_color.compare(   group->get_color_name(   )  )==0  ){
                return it.value(   );
            }
        }
    }
}

bool MatrixWidget::is_valid_point(  QPoint pos )
{
    bool ret = false;
    if (  (  pos.x(   )>-1 ) && (  pos.y(   ) > -1 ) ){
        ret = true;
    }
    return ret;
}

void MatrixWidget::produce_style(  const int rows, const int cols, const int w, const int h )
{
    QVector<MatrixCellInfo> cell_list = MatrixFactory::Instance(   )->produce_layout(  rows, cols, w, h );
    int count = cell_list.size(   );

    QMutexLocker locker(  &m_layout_info_map_lock );
    m_layout_info_map.clear(   );
    for(  int i=0; i<count; i++ )
    {
        MatrixLayoutInfo_S info;
        info.row            = cell_list[i].row;
        info.col            = cell_list[i].col;
        info.index          = i;
        info.color          = Qt::white; // 默认白色
        info.rect           = cell_list[i].rect;
        info.select_status  = 0;
        m_layout_info_map[i] = info;
    }
    update(   );
}

bool MatrixWidget::get_cell_info_by_rect(  QPoint begin_pos, QPoint end_pos, QMap<int, MatrixLayoutInfo_S> &cell_list )
{
    // 反画不允许
    if (   m_draw_end_pos.x(   ) < m_draw_start_pos.x(   ) && m_draw_end_pos.y(   ) < m_draw_start_pos.y(   )  )
    {
        return false;
    }

    MatrixLayoutInfo_S begin_cell;
    MatrixLayoutInfo_S end_cell;
    int begin_index = get_cell_info_by_pos(  begin_pos, &begin_cell );
    int end_index = get_cell_info_by_pos(  end_pos, &end_cell );
    if (  (  begin_index != -1 ) && (  end_index != -1 ) )
    {
        for(  int i=begin_cell.row; i<=end_cell.row; i++ )
        {
            for(  int j=begin_cell.col; j<=end_cell.col; j++ )
            {
                MatrixLayoutInfo_S info = get_cell_info(  i, j );
                // 单元格已经存在组，不可以操作
                if (   info.group_index != -1  )
                {
                    // qDebug(   ) << "选择的单元格， 已经分组， 不能重复选中";

                    return false;
                }
                cell_list[info.index] = info;
            }
        }
    }


    return true;

}

MatrixGroup * MatrixWidget::add_cell_to_group_map(  QMap<int, MatrixLayoutInfo_S>  cell_map_info, bool changed )
{
    MatrixGroup* ret_group = 0;
    MatrixGroup* group = is_exist_not_used_group(   );
    if (  group ){
        QMap<int, MatrixLayoutInfo_S>::iterator it = cell_map_info.begin(   );
        for(  ; it !=cell_map_info.end(   ); it++  ){
            MatrixLayoutInfo_S info = it.value(   );
            info.select_status      = 1;
            info.color              = group->get_group_color(   );
            info.group_index        = group->get_group_id(   );
            QMap<int, MatrixLayoutInfo_S> cell_map;
			cell_map[info.index] = info;
			// 更新到单元格信息中
			m_layout_info_map[info.index] = info;

            // 更新到组中
            group->add_cell(  info );
        }

        ret_group = group;
    }else{
        QMessageBox::information(  this, tr(  "系统提示" ), tr(  "只支持四组区域选择！" ) );
    }

    // 颜色
    return ret_group;
}

MatrixGroup *MatrixWidget::add_cell_to_group_map(  int group_index, MatrixLayoutInfo_S cell_info, bool changed  )
{
    MatrixGroup * ret_group = 0;
    MatrixGroup* group = get_group_by_group_id(  group_index );
    if (  group ){

		cell_info.select_status = 1;
        cell_info.color = group->get_group_color(   );
        cell_info.group_index = group->get_group_id(   );
		QMap<int, MatrixLayoutInfo_S> cell_map;
		cell_map[cell_info.index] = cell_info;
		// 更新到单元格信息中
		m_layout_info_map[cell_info.index] = cell_info;

        // 更新到组中
        group->add_cell(  cell_info );

        ret_group = group;
    }
    return ret_group;
}

MatrixGroup * MatrixWidget::del_cell_from_group_map( int group_index, int cell_index )
{
    MatrixGroup * ret_group = 0;
    MatrixGroup* group = get_group_by_group_id( group_index );
    if ( group ){
        // 不是预置点客户端， 只能对自己画的分析进行操作
        if (  MSCClientType::Instance(  )->get_client_type(  ) != ClientPresetSet )
        {
            if (  ClientPresetSet == group->me_ana_of_client  ){
                return 0;
            }
        }
        // 更新到组中
        group->del_cell( cell_index );
        // 更新到单元格信息中
        MatrixLayoutInfo_S info = m_layout_info_map[cell_index];
        info.group_index = -1;
        info.select_status = 0;
        info.color = Qt::white;
        m_layout_info_map[cell_index] = info;

       ret_group = group;
    }
    return ret_group;
}

MatrixGroup* MatrixWidget::is_exist_not_used_group(  )
{
    MatrixGroup* ret_group = 0;
    QMutexLocker locker( &m_cell_group_list_lock );
    for( int i=0; i<m_cell_group_list.size(  ); i++  ){
        MatrixGroup* group = m_cell_group_list.at( i );
        if ( !group->is_used(  ) ){
            ret_group = group;
            break;
        }
    }
    return ret_group;
}

MatrixGroup*  MatrixWidget::get_group_by_group_id( int group_id )
{
    MatrixGroup* ret_group = 0;
    QMutexLocker locker( &m_cell_group_list_lock );
    for( int i=0; i<m_cell_group_list.size(  ); i++  ){
        MatrixGroup* group = m_cell_group_list.at( i );
        if ( group->get_group_id(  ) == group_id ){
            ret_group = group;
            break;
        }
    }
    return ret_group;
}

MatrixGroup *MatrixWidget::get_group_by_group_name( const QString group_name )
{
    MatrixGroup* ret_group = 0;
    QMutexLocker locker( &m_cell_group_list_lock );
    for( int i=0; i<m_cell_group_list.size(  ); i++  ){
        MatrixGroup* group = m_cell_group_list.at( i );
        if ( group->get_group_name(  ).compare( group_name ) == 0 ){
            ret_group = group;
            break;
        }
    }
    return ret_group;
}

void MatrixWidget::paintEvent( QPaintEvent *event )
{
    QPainter draw( this );
    draw.setPen(  Qt::black  );

    QMutexLocker locker( &m_layout_info_map_lock );
    QMap<int, MatrixLayoutInfo_S>::iterator it = m_layout_info_map.begin(  );
    while ( it != m_layout_info_map.end(  ) ){
        MatrixLayoutInfo_S info = it.value(  );
        if ( m_matrix_select_mode_e == MATRIXSELECTMODE_NEW )
        {
            if ( is_valid_point( m_draw_start_pos )&&
                is_valid_point( m_draw_end_pos ) ) {

                draw.setPen(  Qt::red  );
                QRect rect( m_draw_start_pos, m_draw_end_pos );
                draw.drawRect( rect );
                m_draw_start_pos.setX( -1 );
                m_draw_start_pos.setY( -1 );
                m_draw_end_pos.setX( -1 );
                m_draw_end_pos.setY( -1 );
            }
        }
        draw.setPen(  Qt::black  );
        if ( info.select_status )
        {
            if ( info.is_alarming ){
                // int value = info.alarm_times % 2;
                draw.fillRect( info.rect.x(  ), info.rect.y(  ), info.rect.width(  ), info.rect.height(  ), info.alarm_times % 2?info.color:Qt::red );
            }else{
                draw.fillRect( info.rect.x(  ), info.rect.y(  ), info.rect.width(  ), info.rect.height(  ), QBrush( info.color )  );
            }
        }
        draw.drawRect( info.rect );

        it++;
    }

    draw.end(  );
}

void MatrixWidget::resizeEvent( QResizeEvent *event )
{
   // qDebug(  ) << "resizeEvent width: " << event->size(  ).width(  ) << "; height: " << event->size(  ).height(  );

    ui->m_btn_matrix_back->move(  width(  )- ui->m_btn_matrix_back->width(  ) - 20,
                            height(  )- ui->m_btn_matrix_back->height(  ) - 20  );

    mf_sz_width = ( float )width(  )/( float )MS_WINDOW_WIDTH;
    mf_sz_height = ( float )height(  )/( float )MS_WINDOW_HEIGHT;

    produce_style( 20, 20, event->size(  ).width(  ), event->size(  ).height(  ) );
}

void MatrixWidget::mousePressEvent( QMouseEvent *event )
{
    if (  event->button(  ) == Qt::LeftButton  ) {

        if (  MSCClientType::Instance(  )->get_client_type(  ) != ClientPresetSet  )
        {
            return;
        }

        MatrixLayoutInfo_S info;
        mn_curr_select_index = get_cell_info_by_pos( event->pos(  ), &info );
        if ( mn_curr_select_index != -1 )
        {
            if ( info.is_new(  ) )
            {
                // 允许拖放
                m_draw_start_pos = event->pos(  );

                // 设置这里，就解决主窗口定时器开着就不能画矩阵的问题
                m_draw_end_pos.setX( -1 );
                m_draw_end_pos.setY( -1 );
                m_matrix_select_mode_e = MATRIXSELECTMODE_NEW;
                // qDebug(  ) << "新增区域";
            }
            else
            {
                m_draw_start_pos.setX( -1 );
                m_draw_start_pos.setY( -1 );
                m_matrix_select_mode_e = MATRIXSELECTMODE_DRAG_DROP;
               // qDebug(  ) << "不是新增区域";
            }
        }
    }
}

void MatrixWidget::mouseReleaseEvent( QMouseEvent *event )
{
    if (  event->button(  ) == Qt::LeftButton  ) {

        if (  MSCClientType::Instance(  )->get_client_type(  ) != ClientPresetSet  )
        {
            return;
        }

        if ( m_matrix_select_mode_e == MATRIXSELECTMODE_NEW )
        {
            // 画矩形区域
            m_draw_end_pos = event->pos(  );
            // 判断区域内选中的单元格
            QMap<int, MatrixLayoutInfo_S> cell_list;
            bool ret = get_cell_info_by_rect( m_draw_start_pos, m_draw_end_pos, cell_list );
            if ( ret && cell_list.size(  ) )
            {
                // 更新到中分组列表中, 同时更新到单元格信息表中
               MatrixGroup* group = add_cell_to_group_map( cell_list );
                // 增加分析到服务器
               if ( group ){
                   group->set_ana_of_client(  MSCClientType::Instance(  )->get_client_type(  )  );
                   group->add_matrix_ana_to_server(  );
               }


                update(  );
            }
        }
        else
        {
            m_drag_start_pos = event->pos(  );
            MatrixLayoutInfo_S info;
            int ret = get_cell_info_by_pos( event->pos(  ), &info );
            if ( ret != -1 )
            {
                // Press 与 Release是同一个单元格,
                if ( ret == mn_curr_select_index ) // 执行到这里， 状态一定已经选中, 状态改为未选中， 颜色白色
                {
                    // 从分组中删除
                    MatrixGroup* group = del_cell_from_group_map( info.group_index, ret );
                    if ( group ){
                        if ( group->is_used(  ) ){
                            group->chg_matrix_ana_to_server(  );
                        }else{
                            group->del_matrix_ana_to_server(  );
                        }
                    }
                    // 更新单元格信息列表
                    m_matrix_select_mode_e = MATRIXSELECTMODE_SINGLE;
                    update(  );
                }
                else
                {
                    // 拖放
                    m_matrix_select_mode_e = MATRIXSELECTMODE_DRAG_DROP;
                }
            }
        }
    }
}

void MatrixWidget::mouseMoveEvent( QMouseEvent *event )
{
    if ( !( event->buttons(  ) & Qt::LeftButton ) ){
        return;
    }

    if (  MSCClientType::Instance(  )->get_client_type(  ) != ClientPresetSet  )
    {
        return;
    }

    if ( ( event->pos(  ) - m_drag_start_pos ).manhattanLength(  ) < QApplication::startDragDistance(  ) ) {
        return;
    }

    if ( m_matrix_select_mode_e != MATRIXSELECTMODE_DRAG_DROP ){
        return;
    }

    MatrixLayoutInfo_S info;
    int ret = get_cell_info_by_pos( event->pos(  ), &info );
    if ( ret == -1 ){
        return;
    }

    if (  info.group_index == -1  ){
        return;
    }

    QPoint point( ( int )info.rect.x(  ), ( int )( info.rect.y(  ) ) );
    QPoint hot_spot = event->pos(  ) - point;


    QByteArray item_data;
    QDataStream dataStream( &item_data, QIODevice::WriteOnly );
    dataStream << info.group_index <<  info.index ; //<< QPoint( hot_spot );

    QMimeData *mime_data = new QMimeData;
    mime_data->setData( "application/x-matrix-draganddrop", item_data );
    QPixmap pixmap( info.rect.size(  ).toSize(  ) );
    pixmap.fill( info.color );
    QDrag *drag = new QDrag( this );
    drag->setMimeData( mime_data );
    drag->setPixmap( pixmap );
    drag->setHotSpot( hot_spot );

    drag->exec( Qt::MoveAction, Qt::MoveAction );
}

void MatrixWidget::dragEnterEvent( QDragEnterEvent *event )
{
    if (  MSCClientType::Instance(  )->get_client_type(  ) != ClientPresetSet  )
    {
        return;
    }

    if ( m_matrix_select_mode_e != MATRIXSELECTMODE_DRAG_DROP ){
        return;
    }


    if ( event->mimeData(  )->hasFormat( "application/x-matrix-draganddrop" ) ){
        if ( children(  ).contains( event->source(  ) ) ){
            event->setDropAction( Qt::MoveAction );
            event->accept(  );
        }
        else{
            event->acceptProposedAction(  );
        }
    }
    else if ( event->mimeData(  )->hasText(  ) ){
        event->acceptProposedAction(  );
    }
    else{
        event->ignore(  );
    }
}

void MatrixWidget::dragMoveEvent( QDragMoveEvent *event )
{
    if (  MSCClientType::Instance(  )->get_client_type(  ) != ClientPresetSet  )
    {
        return;
    }

    if ( m_matrix_select_mode_e != MATRIXSELECTMODE_DRAG_DROP ){
        return;
    }

    if ( event->mimeData(  )->hasFormat( "application/x-matrix-draganddrop" ) ){
        if ( children(  ).contains( event->source(  ) ) ) {
            event->setDropAction( Qt::MoveAction );
            event->accept(  );
        }
        else {
            event->acceptProposedAction(  );
        }
    }
    else if ( event->mimeData(  )->hasText(  ) ){
        event->acceptProposedAction(  );
    }
    else{
        event->ignore(  );
    }
}

void MatrixWidget::dropEvent( QDropEvent *event )
{
    if (  MSCClientType::Instance(  )->get_client_type(  ) != ClientPresetSet  )
    {
        return;
    }

    if ( m_matrix_select_mode_e != MATRIXSELECTMODE_DRAG_DROP ) {
        return;
    }

    if ( event->mimeData(  )->hasFormat( "application/x-matrix-draganddrop" ) ){
       const QMimeData *mime = event->mimeData(  );
       QByteArray itemData = mime->data( "application/x-matrix-draganddrop" );
       QDataStream dataStream( &itemData, QIODevice::ReadOnly );

       int group_index;
       int source_index;
       QPoint offset;
       dataStream >> group_index >> source_index >> offset;



       if ( event->source(  ) == this ){
           MatrixLayoutInfo_S target_info;
           int ret = get_cell_info_by_pos( event->pos(  ), &target_info );
           if ( ret != -1 ){
               // source和target是同一个return;
               if ( target_info.index == source_index ){
                   // qDebug(  ) << "源和目标是同一个单元格, 拖放失败";
                   return;
               }

               if ( target_info.group_index != -1 ){
                   // qDebug(  ) << "已经存在组，不可以拖放";
                   return;
               }

               MatrixGroup* group = get_group_by_group_id( group_index );
               if ( group )
               {
                   // 不是预置点客户端， 只能对自己画的分析进行操作
                   if (  MSCClientType::Instance(  )->get_client_type(  ) != ClientPresetSet )
                   {
                       if (  ClientPresetSet == group->me_ana_of_client  ){
                           return;
                       }
                   }
                   target_info.group_index = group_index;
                   target_info.select_status = 1;
                   target_info.color = group->get_group_color(  );
                   // 加入单元格消息列表
                   QMutexLocker locker( &m_layout_info_map_lock );
                   m_layout_info_map[target_info.index] = target_info;

                   // 加入组列表
                   MatrixGroup* group = add_cell_to_group_map( group_index, target_info, true );
                   if ( group ){
                       group->chg_matrix_ana_to_server(  );
                   }

                   event->setDropAction( Qt::MoveAction );
                   event->accept(  );

                   update(  );
               }
           }
       }
       else{
           event->acceptProposedAction(  );
       }
    }
    else{
       event->ignore(  );
    }
}

void MatrixWidget::contextMenuEvent( QContextMenuEvent *event )
{
    QCursor cur=this->cursor(  );

    m_menu->exec( cur.pos(  ) );
}

int MatrixWidget::get_cell_info_by_pos( QPoint pos, MatrixLayoutInfo_S *out_info )
{
    int ret = -1;
    QMutexLocker locker( &m_layout_info_map_lock );
    QMap<int, MatrixLayoutInfo_S>::iterator it = m_layout_info_map.begin(  );
    while ( it != m_layout_info_map.end(  ) ){
        MatrixLayoutInfo_S info = it.value(  );

        if(  ( pos.x(  ) > info.rect.topLeft(  ).x(  ) ) &&
           ( pos.y(  ) > info.rect.topLeft(  ).y(  ) ) &&
           ( pos.x(  ) <= info.rect.bottomRight(  ).x(  ) ) &&
           ( pos.y(  ) <= info.rect.bottomRight(  ).y(  ) )  ){
            ret = info.index;
            if ( out_info ){
                *out_info = info;
            }
            break;
        }

        it++;
    }
    return ret;
}

MatrixLayoutInfo_S MatrixWidget::get_cell_info( int row, int col )
{
    MatrixLayoutInfo_S ret_info;
    QMutexLocker locker( &m_layout_info_map_lock );
    QMap<int, MatrixLayoutInfo_S>::iterator it = m_layout_info_map.begin(  );
    for ( ; it != m_layout_info_map.end(  ); it++ ){
        MatrixLayoutInfo_S info = it.value(  );
        if ( info.row==row && info.col==col ){
            ret_info = info;
            break;
        }
    }
    return ret_info;
}

void MatrixWidget::slot_style_20_20(  )
{
    produce_style( 20, 20, width(  ), height(  ) );
}

//void MatrixWidget::slot_style_18_18(  )
//{
//    produce_style( 18, 18, width(  ), height(  ) );
//}

//void MatrixWidget::slot_style_16_16(  )
//{
//    produce_style( 16, 16, width(  ), height(  ) );
//}

//void MatrixWidget::slot_style_10_10(  )
//{
//    produce_style( 10, 10, width(  ), height(  ) );
//}

//void MatrixWidget::slot_style_8_8(  )
//{
//    produce_style( 8, 8, width(  ), height(  ) );
//}

//void MatrixWidget::slot_style_6_6(  )
//{
//    produce_style( 6, 6, width(  ), height(  ) );
//}

void MatrixWidget::go_back_slot(  )
{
    // 退出的时候， 自动保存， 矩阵分析信息


//    DeviceGroup  devGroup;
//    CDataMode::Instance(  )->get_group_info( mn_group_number,devGroup );
//    if ( !devGroup.dev_ir.str_sn.isEmpty(  ) )
//    {
//        QMutexLocker locker( &m_cell_group_list_lock );
//        for( int i=0; i < m_cell_group_list.size(  ); i++ )
//        {
//            MatrixGroup* group = m_cell_group_list.at( i );
//            // "R:x,y;x,y;-R:x,y;x,y;-"
//            QString temp_str = "";
//            int cell_count = group->m_cell_list.size(  );
//            if (  cell_count <= 0  ) return;
//            for( int j=0; j<cell_count; j++ ){
//                MatrixLayoutInfo_S cell = group->m_cell_list.at( i );
//                // 按比例换算
//                QPoint pt_start = QPoint(  ( int )(  cell.rect.topLeft(  ).x(  )/mf_sz_width  ), ( int )(  cell.rect.topLeft(  ).y(  )/mf_sz_height  )  );
//                QPoint pt_end = QPoint(  ( int )(  cell.rect.bottomRight(  ).x(  )/mf_sz_width  ), ( int )(  cell.rect.bottomRight(  ).y(  )/mf_sz_height  )  );

//                temp_str += "R:" + QString::number(  ( int )pt_start.x(  )  ) + "," + QString::number(   ( int )pt_start.y(  )   ) + ";"
//                        + QString::number(  ( int )pt_end.x(  )  ) + "," + QString::number(  ( int )pt_end.y(  )  ) + ";-";


////                temp_str += "R:" + QString::number(  ( int )cell.rect.topLeft(  ).x(  )  ) + "," + QString::number(   ( int )cell.rect.topLeft(  ).y(  )   ) + ";"
////                        + QString::number(  ( int )cell.rect.bottomRight(  ).x(  )  ) + "," + QString::number(  ( int )cell.rect.bottomRight(  ).y(  )  ) + ";-";
//            }

//            // 新增时， n_id_ana不知道
//            AnaItem ana_item;
//            ana_item.n_id_ana = -1;
//            ana_item.str_name = group->get_group_name(  );
//            ana_item.str_ana = temp_str;

//            qDebug(  ) << "上传服务前：" << temp_str;
//            // 从m_group_ana_id_list查找组, 找到组查看， 这个组是否更动过
//            QMap<MatrixGroup*, int>::iterator it = m_group_ana_id_list.find( group );
//            if (  it != m_group_ana_id_list.end(  )  ){

//                if ( !group->is_used(  ) )
//                    return;

//                if ( it.value(  ) == -1 ){
//                    // 新加
//                    add_preset_ana( mn_group_number, mn_rail, mn_ptz, ana_item.str_ana, ana_item.str_name );
//                }else if( it.key(  )->is_changed(  ) ){
//                    // 更改
//                    int n_ana_id = it.value(  ); // 分析ID
//                    ana_item.n_id_ana = n_ana_id;
//                    if ( !it.key(  )->is_used(  ) ){
//                        // 列表为空， 就删除矩阵分析
//                        delete_preset_ana( mn_group_number, mn_rail, mn_ptz, n_ana_id );
//                    }else{
//                        change_preset_ana( mn_group_number, mn_rail, mn_ptz, n_ana_id, ana_item.str_ana, ana_item.str_name );
//                    }
//                }
//            }
//        } // end out for
//    }
    emit matrix_widget_hide(  );
    hide(  );
}

void MatrixWidget::clear_act(  )
{
    int inc_flag = 0;
    MatrixGroup* group_1  = m_cell_group_list[0];
    if (  group_1  ){
        if (  !group_1->is_used(  )  ){
            m_green_act->setVisible( false );
        }else{
            m_green_act->setVisible( true );
            inc_flag++;
        }
    }
    MatrixGroup* group_2  = m_cell_group_list[1];
    if (  group_2  ){
        if (  !group_2->is_used(  )  ){
            m_cyan_act->setVisible( false );
        }else{
            m_cyan_act->setVisible( true );
            inc_flag++;
        }
    }

    MatrixGroup* group_3  = m_cell_group_list[2];
    if (  group_3  ){
        if (  !group_3->is_used(  )  ){
            m_blue_act->setVisible( false );
        }else{
            m_blue_act->setVisible( true );
            inc_flag++;
        }
    }

    MatrixGroup* group_4  = m_cell_group_list[3];
    if (  group_4  ){
        if (  !group_4->is_used(  )  ){
            m_yellow_act->setVisible( false );
        }else{
            m_yellow_act->setVisible( true );
            inc_flag++;
        }
    }

    if ( inc_flag > 0 ){
        m_clear_act->setVisible(  true  );
    }else{
        m_clear_act->setVisible(  false  );
    }
}

void MatrixWidget::clear_green_slot(  )
{
    MatrixGroup* group = m_cell_group_list[0];
    if ( group ){

        if (  MSCClientType::Instance(  )->get_client_type(  ) != ClientPresetSet )
        {
            if (  ClientPresetSet == group->me_ana_of_client  ){
                QMessageBox::information( this, tr( "系统提示" ), tr( "不是本客户端画的分析不可以清除" ) );
                return;
            }
        }

        // 前清除
        group->clear(  );
        matrix_clear( group->get_group_id(  ) );
        // 后从服务中删除
        int ana_id = m_group_ana_id_list[group];
        delete_preset_ana( mn_group_number, mn_rail,mn_ptz, ana_id );

        update(  );
    }
}

void MatrixWidget::clear_yarn_slot(  )
{
    MatrixGroup* group = m_cell_group_list[1];
    if ( group ){
        if (  MSCClientType::Instance(  )->get_client_type(  ) != ClientPresetSet )
        {
            if (  ClientPresetSet == group->me_ana_of_client  ){
                QMessageBox::information( this, tr( "系统提示" ), tr( "不是本客户端画的分析不可以清除" ) );
                return;
            }
        }

        // 前清除
        group->clear(  );
        matrix_clear( group->get_group_id(  ) );
        // 后从服务中删除
        int ana_id = m_group_ana_id_list[group];
        delete_preset_ana( mn_group_number, mn_rail,mn_ptz, ana_id );

        update(  );
    }
}

void MatrixWidget::clear_blue_slot(  )
{
    MatrixGroup* group = m_cell_group_list[2];
    if ( group ){
        if (  MSCClientType::Instance(  )->get_client_type(  ) != ClientPresetSet )
        {
            if (  ClientPresetSet == group->me_ana_of_client  ){
                QMessageBox::information( this, tr( "系统提示" ), tr( "不是本客户端画的分析不可以清除" ) );
                return;
            }
        }

        // 前清除
        group->clear(  );
        matrix_clear( group->get_group_id(  ) );
        // 后从服务中删除
        int ana_id = m_group_ana_id_list[group];
        delete_preset_ana( mn_group_number, mn_rail,mn_ptz, ana_id );

        update(  );
    }
}

void MatrixWidget::clear_yellow_slot(  )
{
    MatrixGroup* group = m_cell_group_list[3];
    if ( group ){
        if (  MSCClientType::Instance(  )->get_client_type(  ) != ClientPresetSet )
        {
            if (  ClientPresetSet == group->me_ana_of_client  ){
                QMessageBox::information( this, tr( "系统提示" ), tr( "不是本客户端画的分析不可以清除" ) );
                return;
            }
        }
        // 前清除
        group->clear(  );
        matrix_clear( group->get_group_id(  ) );
        // 后从服务中删除
        int ana_id = m_group_ana_id_list[group];
        delete_preset_ana( mn_group_number, mn_rail,mn_ptz, ana_id );

        update(  );
    }
}

void MatrixWidget::clear_all_region_slot()
{
    clear_matrix_ana_info();
}

//2015.3.4 blm add 预置点删除时清除矩阵区域
void MatrixWidget::clear_matrix_ana_info(  )
{
    clear_green_slot(  );
    clear_yarn_slot(  );
    clear_blue_slot(  );
    clear_yellow_slot(  );
}

int MatrixWidget::onResponse( CBaseMsg &reqMsg, CBaseMsg &rspMsg )
{
    CIrPreview::onResponse( reqMsg, rspMsg );
    switch(  rspMsg.m_nCmd  ) {
    case PresetAnaAdd:
    {
        if (  rspMsg.m_nResult == ResultOk  ) {
            int ana_id = rspMsg.map_msgdata[MSGDATA_ANA_ID].value<int>(  );
            AnaItem ana_item;
            ana_item.n_id_ana = ana_id;
            ana_item.str_ana = reqMsg.map_msgdata[MSGDATA_STR_ANA].toString(  );
            // 分析名就是组名， 因为当时附值的就是组名
            QString ana_name = reqMsg.map_msgdata[MSGDATA_NAME].toString(  );
            ana_item.str_name = ana_name;
            MatrixGroup* group = get_group_by_group_name( ana_name );
            if ( group ){
                m_group_ana_id_list[group] = ana_id;
                emit sig_add_preset_matrix_ana_to_server_success(  ana_item, mn_rail, mn_ptz  );
            }
        }
        break;
    }
    case PresetAnaChange:
    {
        if (  rspMsg.m_nResult == ResultOk  ) {
            int ana_id = reqMsg.map_msgdata[MSGDATA_ANA_ID].value<int>(  );
            AnaItem ana_item;
            ana_item.n_id_ana = ana_id;
            ana_item.str_ana = reqMsg.map_msgdata[MSGDATA_STR_ANA].toString(  );
            // 分析名就是组名， 因为当时附值的就是组名
            QString ana_name = reqMsg.map_msgdata[MSGDATA_NAME].toString(  );
            ana_item.str_name = ana_name;

            MatrixGroup* group = get_group_by_group_name( ana_name );
            if ( group ){
                m_group_ana_id_list[group] = ana_id;
                emit sig_chg_preset_matrix_ana_to_server_success(  ana_item, mn_rail, mn_ptz  );
            }
        }
        break;
    }
    case PresetAnaDel:
    {
        if (  rspMsg.m_nResult == ResultOk  ) {
            int ana_id = reqMsg.map_msgdata[MSGDATA_ANA_ID].value<int>(  );
            AnaItem ana_item;
            ana_item.n_id_ana = ana_id;
            ana_item.str_ana = reqMsg.map_msgdata[MSGDATA_STR_ANA].toString(  );
            // 分析名就是组名， 因为当时附值的就是组名
            ana_item.str_name = reqMsg.map_msgdata[MSGDATA_NAME].toString(  );

            QMap<MatrixGroup*, int>::iterator it = m_group_ana_id_list.begin(  );
            while (  it != m_group_ana_id_list.end(  )  ){
                if (  ana_id == it.value(  ) ){
                    it = m_group_ana_id_list.erase( it );
                    emit sig_del_preset_matrix_ana_to_server_success(  ana_item, mn_rail, mn_ptz  );
                }else{
                    it++;
                }

            }
            emit sig_update_alarm_info();
        }
        break;
    }
    }
    return 0;

}


bool MatrixGroup::add_matrix_ana_to_server(  )
{
    bool ret = true;

    int cell_count = m_cell_list.size(  );
    if (  cell_count <= 0  ) return false;
    QString temp_str;
    for( int j=0; j<cell_count; j++ ){
        MatrixLayoutInfo_S cell = m_cell_list.at( j );
        // 按比例换算
        QPoint pt_start = QPoint(  ( int )(  cell.rect.topLeft(  ).x(  )/mw_matrix_widget->mf_sz_width  ),
                                  ( int )(  cell.rect.topLeft(  ).y(  )/mw_matrix_widget->mf_sz_height  )  );
        QPoint pt_end = QPoint(  ( int )(  cell.rect.bottomRight(  ).x(  )/mw_matrix_widget->mf_sz_width  ),
                                ( int )(  cell.rect.bottomRight(  ).y(  )/mw_matrix_widget->mf_sz_height  )  );
        temp_str += "R:" + QString::number(  ( int )pt_start.x(  )  ) + "," + QString::number(   ( int )pt_start.y(  )   ) + ";"
                + QString::number(  ( int )pt_end.x(  )  ) + "," + QString::number(  ( int )pt_end.y(  )  ) + ";-";

//          temp_str += "R:" + QString::number(  ( int )cell.rect.topLeft(  ).x(  )  ) + "," + QString::number(   ( int )cell.rect.topLeft(  ).y(  )   ) + ";"
//                        + QString::number(  ( int )cell.rect.bottomRight(  ).x(  )  ) + "," + QString::number(  ( int )cell.rect.bottomRight(  ).y(  )  ) + ";-";
    }
   // qDebug(  ) << "add_matrix_ana_to_server size:"<< temp_str.length(  ) <<";  str:"<<temp_str;

    qDebug() << "temp_str.length:"<<temp_str.length();
    qDebug() << temp_str;
    mw_matrix_widget->add_preset_ana( mw_matrix_widget->mn_group_number, mw_matrix_widget->mn_rail,
                                     mw_matrix_widget->mn_ptz, temp_str, ms_name );


    return ret;
}

bool MatrixGroup::chg_matrix_ana_to_server(  )
{
    bool ret = true;

    int cell_count = m_cell_list.size(  );
    if (  cell_count <= 0  ) return false;
    QString temp_str;
    for( int j=0; j<cell_count; j++ ){
        MatrixLayoutInfo_S cell = m_cell_list.at( j );
        // 按比例换算
        QPoint pt_start = QPoint(  ( int )(  cell.rect.topLeft(  ).x(  )/mw_matrix_widget->mf_sz_width  ),
                                  ( int )(  cell.rect.topLeft(  ).y(  )/mw_matrix_widget->mf_sz_height  )  );
        QPoint pt_end = QPoint(  ( int )(  cell.rect.bottomRight(  ).x(  )/mw_matrix_widget->mf_sz_width  ),
                                ( int )(  cell.rect.bottomRight(  ).y(  )/mw_matrix_widget->mf_sz_height  )  );
        temp_str += "R:" + QString::number(  ( int )pt_start.x(  )  ) + "," + QString::number(   ( int )pt_start.y(  )   ) + ";"
                + QString::number(  ( int )pt_end.x(  )  ) + "," + QString::number(  ( int )pt_end.y(  )  ) + ";-";

//          temp_str += "R:" + QString::number(  ( int )cell.rect.topLeft(  ).x(  )  ) + "," + QString::number(   ( int )cell.rect.topLeft(  ).y(  )   ) + ";"
//                        + QString::number(  ( int )cell.rect.bottomRight(  ).x(  )  ) + "," + QString::number(  ( int )cell.rect.bottomRight(  ).y(  )  ) + ";-";
    }
    int ana_id = mw_matrix_widget->get_ana_id_by_group_index(  this  );
    mw_matrix_widget->change_preset_ana( mw_matrix_widget->mn_group_number, mw_matrix_widget->mn_rail,
                                        mw_matrix_widget->mn_ptz, ana_id, temp_str, ms_name );
    return ret;
}

bool MatrixGroup::del_matrix_ana_to_server(  )
{
    bool ret = true;
    int ana_id = mw_matrix_widget->get_ana_id_by_group_index(  this  );
    mw_matrix_widget->delete_preset_ana( mw_matrix_widget->mn_group_number, mw_matrix_widget->mn_rail,
                                        mw_matrix_widget->mn_ptz, ana_id );
    return ret;

}

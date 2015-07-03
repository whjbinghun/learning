#include "wndmatrixcnd.h"
#include "ui_wndmatrixcnd.h"
#include <QLabel>
#include "wndcolor.h"
#include <QColorDialog>
#include <QMessageBox>
#include "alarmcfg.h"

WndMatrixCnd::WndMatrixCnd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WndMatrixCnd)
    ,mp_alarm_cond_dlg( NULL )
{
    ui->setupUi(this);
    init_ctrl();

    init_connect();
}

WndMatrixCnd::~WndMatrixCnd()
{
    delete ui;
}

void WndMatrixCnd::init_ctrl()
{
    ui->centralWidget->setMouseTracking( true );
    setMouseTracking( true );
    ui->id_tablewidget_matrix_cnd->setMouseTracking( true );
    ui->id_tablewidget_matrix_cnd->init_lab( ui->id_lab_martix_cnd );

    mp_alarm_cond_dlg = new DlgAlarmCondition( this );

    connect( ui->id_ir_frame->get_matrix_widget(), SIGNAL(sig_matrix_ana_info(QString,QColor)), \
             ui->id_tablewidget_matrix_cnd, SLOT( slot_martix_ana_info( QString, QColor ) ) );
}

void WndMatrixCnd::init_connect()
{
    connect( ui->id_ptn_add_matrix, SIGNAL( clicked() ), this, SLOT( slot_add_martix() ) );
    connect( ui->id_ptn_del_martix, SIGNAL( clicked() ), this, SLOT( slot_del_martix() ) );
    connect( ui->id_ptn_add_matrix_cnd, SIGNAL( clicked() ), this, SLOT( slot_add_martix_cnd() ) );
    connect( ui->id_ptn_del_matrix_cnd, SIGNAL( clicked() ), this, SLOT( slot_del_martix_cnd() ) );




    //connect( ui->id_pre_btn_add, SIGNAL( clicked() ), this, SLOT( slot_click_btn_add() ));
    //connect( ui->id_pre_btn_delete, SIGNAL( clicked() ), this, SLOT( slot_click_btn_delete() ));
    //connect( ui->id_pre_btn_modify, SIGNAL( clicked() ), this, SLOT( slot_click_btn_modify() ));
}


void WndMatrixCnd::slot_add_martix()
{
    QString str_martix_name = "矩阵";
    QColor clr = Qt::blue;
    QPixmap pix_map;
    QString str_martix_cnd = "报警条件";

    /*int n_row = ui->id_tablewidget_matrix_cnd->rowCount();

    ui->id_tablewidget_matrix_cnd->insertRow( n_row );//行号
    ui->id_tablewidget_matrix_cnd->setItem( n_row, 0, new QTableWidgetItem(  QString::number( n_row ) ) );//第n_preset_row_num+1行，第1列
    ui->id_tablewidget_matrix_cnd->setItem( n_row, 1, new QTableWidgetItem( str_martix_name ) );

    WndColor *p_widget_color = new WndColor( ui->id_tablewidget_matrix_cnd );
    p_widget_color->set_color( Qt::blue );
    p_widget_color->set_row( n_row );
    ui->id_tablewidget_matrix_cnd->setCellWidget( n_row, 2, p_widget_color );
    connect( p_widget_color, SIGNAL( sig_color_dialog() ), this, SLOT( slot_color_dialog() ) );

    ui->id_tablewidget_matrix_cnd->setItem( n_row, 3, new QTableWidgetItem( str_martix_cnd ) );
    */

    MartixInfo *p_martix_info = new MartixInfo;
    p_martix_info->str_martix_name = str_martix_name;
    p_martix_info->clr_martix = clr;
    p_martix_info->str_martix_alarm_cnd = str_martix_cnd;
    ui->id_tablewidget_matrix_cnd->add_martix_tab_list( *p_martix_info );
    //set_martix_list_cur( ui->id_tablewidget_matrix_cnd->rowCount() - 1 );

}

void WndMatrixCnd::slot_del_martix()
{
    //删除选中的矩阵
    ui->id_tablewidget_matrix_cnd->del_martix_tab();
}

void WndMatrixCnd::slot_add_martix_cnd()
{
    //PresetInfo *p_preset_info=NULL;
    //if( (p_preset_info = ui->id_pre_wnd_preset_list->get_double_preset_info()) == NULL ) return;
    AlarmGroup *p_alarm_group;
    QStringList str_list, str_server_list;
    mp_alarm_cond_dlg->show();
    int n_row = ui->id_tablewidget_matrix_cnd->currentRow();
    //ui->id_tablewidget_matrix_cnd->get_
    if( n_row < 0 ) return;
    QString str_martix_name = ui->id_tablewidget_matrix_cnd->item( n_row, 1 )->text();
    QStringList str_alarm_list;
    str_alarm_list.push_back( str_martix_name );
    mp_alarm_cond_dlg->set_ana_info( str_alarm_list );

    //QStringList str_alarm_list = get_preset_anas_to_stringlist( p_preset_info );  //获取预置点分析项
    //if( str_alarm_list.size() == 0 )  return;
    //int n_current_tab = ui->id_pre_tab_widget->currentIndex();
    //if( n_current_tab == 0 ){
    //    mp_alarm_cond_dlg->set_matrix_combo_enable( true );
    //} else if( n_current_tab == 1 ){
        mp_alarm_cond_dlg->set_matrix_combo_enable( false );
    //}
    //mp_alarm_cond_dlg->set_ana_info(str_alarm_list); //combox初始化
    mp_alarm_cond_dlg->set_check_general_enable( true );
    mp_alarm_cond_dlg->set_check_array_enable( true );

    if( QDialog::Accepted == mp_alarm_cond_dlg->exec() )
    {



         /*QString str_condition_show,str_conditionsever;
         p_alarm_group = &( mp_alarm_cond_dlg->m_alarm_group );
         if( !mp_alarm_cond_dlg->mb_matrix_ana ){
             //mp_tab_general->alarm_group_to_string( p_alarm_group, str_condition_show );

             if(str_condition_show.contains("OR")>0||str_condition_show.contains("AND")>0){
                 str_list = str_condition_show.split("OR");
                 if(str_list.size()>1){
                     for(int i = 0; i < str_list.size(); i++)  {
                         change_id_to_serverid(str_list[i].trimmed(),str_conditionsever);
                         str_server_list.append(str_conditionsever);
                     }
                     str_conditionsever = str_server_list.join(" OR ");

                 }else{
                     str_list = str_condition_show.split("AND");
                     for(int i = 0; i < str_list.size(); i++)  {
                         change_id_to_serverid(str_list[i].trimmed(),str_conditionsever);
                         str_server_list.append(str_conditionsever);
                     }
                     str_conditionsever = str_server_list.join(" AND ");
                 }

             }else{
                 change_id_to_serverid(str_condition_show,str_conditionsever);
             }

             add_preset_alarm( mn_group_num, p_preset_info->n_rail_preset_no, p_preset_info->n_ptz_preset_no, str_conditionsever, "" );
         } else {
             mp_tab_array->alarm_group_to_string( p_alarm_group, str_condition_show );
             change_matrixid_to_serverid(str_condition_show,str_conditionsever);
             add_preset_alarm( mn_group_num, p_preset_info->n_rail_preset_no, p_preset_info->n_ptz_preset_no, str_conditionsever, "matrix" );

         }*/
        //mp_alarm_cond_dlg->clear_row_info();
        mp_alarm_cond_dlg->hide();
    } else {
        //mp_alarm_cond_dlg->clear_row_info();
        mp_alarm_cond_dlg->hide();
    }

    /*//修改预置点报警条件
     *  int n_current_tab = ui->id_pre_tab_widget->currentIndex();
    PresetInfo* p_preset_info = ui->id_pre_wnd_preset_list->get_double_preset_info();
    if( !p_preset_info ) return;

    if( n_current_tab == 0 ){
        change_general_preset_alarm( p_preset_info );
    } else if( n_current_tab == 1 ){
        change_matrix_preset_alarm( p_preset_info );
    }*/
}

void WndMatrixCnd::slot_del_martix_cnd()
{
    //int n_current_tab = ui->id_pre_tab_widget->currentIndex();
    /*PresetInfo *p_preset_info=NULL;
    QList<AlarmGroup*> alarm_group_list;
    AlarmGroup *p_alarm_group;
    if( (p_preset_info = ui->id_pre_wnd_preset_list->get_double_preset_info()) == NULL ) return;
    if( n_current_tab == 0 ){
        alarm_group_list = mp_tab_general->get_all_selected_alarm_group();   //获得所有要删除的项
    } else if( n_current_tab == 1 ){
        alarm_group_list = mp_tab_array->get_all_selected_alarm_group();   //获得所有要删除的项
    }

    for( QList<AlarmGroup*>::iterator it_alarm = alarm_group_list.begin(); it_alarm != alarm_group_list.end(); it_alarm++ ){
        p_alarm_group = *it_alarm;
        delete_preset_alarm( mn_group_num, p_preset_info->n_rail_preset_no, p_preset_info->n_ptz_preset_no, p_alarm_group->n_alarm_id );
    }*/
}

void WndMatrixCnd::mouseMoveEvent( QMouseEvent *event )
{
    //确定鼠标在报警条件内
    //循环使用报警列表，当鼠标在报警条件位置时，显示一个lab

}

void WndMatrixCnd::set_anaitem_to_stringlist( AnaItem &ana_item, QStringList &s_alarm_list )
{
    QMap< int, int >::iterator it_id;

    QString str_alarm, str_alarm_item;

    if( ana_item.str_name.isEmpty() )
    {
        for (it_id = m_map_id.begin(); it_id != m_map_id.end(); ++it_id ){
            if(it_id.value() == ana_item.n_id_ana){
               str_alarm.setNum(it_id.key());
               break;
            }
        }
        if(ana_item.str_ana.left(1)=="P")
        {
            str_alarm_item = "P" + str_alarm + "[MAX]";
            s_alarm_list.push_back( str_alarm_item );
        }else if(ana_item.str_ana.left(1)=="L"){
            str_alarm_item = "L" +str_alarm + "[MAX]";
            s_alarm_list.push_back( str_alarm_item );
            str_alarm_item = "L" +str_alarm + "[MIN]";
            s_alarm_list.push_back( str_alarm_item );
            str_alarm_item = "L" +str_alarm + "[AVG]";
            s_alarm_list.push_back( str_alarm_item );
        }else if(ana_item.str_ana.left(1)=="R"){
            str_alarm_item = "R" +str_alarm + "[MAX]";
            s_alarm_list.push_back( str_alarm_item );
            str_alarm_item = "R" +str_alarm + "[MIN]";
            s_alarm_list.push_back( str_alarm_item );
            str_alarm_item = "R" +str_alarm + "[AVG]";
            s_alarm_list.push_back( str_alarm_item );
        }
    }else{
        if(ana_item.str_name.left(6) == "matrix"){
            get_matrix_color_name_by_ana_id( ana_item.n_id_ana, str_alarm ) ;
            m_array_id.insert(ana_item.n_id_ana,str_alarm);
            str_alarm_item = str_alarm + "[MAX]";
            s_alarm_list.push_back( str_alarm_item );
            str_alarm_item = str_alarm + "[MIN]";
            s_alarm_list.push_back( str_alarm_item );
            str_alarm_item = str_alarm + "[AVG]";
            s_alarm_list.push_back( str_alarm_item );
        }else{
            str_alarm =ana_item.str_name;
            if(ana_item.str_ana.left(1)=="P")
            {
                str_alarm_item = str_alarm + "[MAX]";
                s_alarm_list.push_back( str_alarm_item );
            }else{
                str_alarm_item = str_alarm + "[MAX]";
                s_alarm_list.push_back( str_alarm_item );
                str_alarm_item = str_alarm + "[MIN]";
                s_alarm_list.push_back( str_alarm_item );
                str_alarm_item = str_alarm + "[AVG]";
                s_alarm_list.push_back( str_alarm_item );
            }
        }
    }
}

void WndMatrixCnd::change_id_to_serverid( QString str_id,QString &str_serverid )
{
    QString str_left,str_right;
    if(str_id.count("[")>1){
        str_left = str_id.mid(0,str_id.indexOf("]")+1);
        str_right = str_id.mid(str_id.indexOf("]")+1,str_id.length()-1);
        replaceidstr( 0, str_left );
        replaceidstr( 3, str_right );
        str_left.append( str_right );
        str_serverid = str_left;
    }else{
        replaceidstr( 0, str_id );
        str_serverid = str_id;
    }

}

void WndMatrixCnd::replaceidstr(int indexnum,QString &str_id)
{
    /*PresetInfo *p_preset_info=NULL;
    if( (p_preset_info = ui->id_pre_wnd_preset_list->get_double_preset_info()) == NULL ) return;
    int n_preset_no = ( p_preset_info->n_rail_preset_no << 16) + p_preset_info->n_ptz_preset_no;
    QMap< int, QMap< int, AnaItem > >::iterator it_preset = m_map_preset_general_ana.find( n_preset_no );
    QMap< int, AnaItem >::iterator it_ana;
    QString strkey,strvalue,strtemp;
    if(str_id.left(1)=="P"||str_id.left(1)=="L"||str_id.left(1)=="R"){
        strkey = str_id.mid(indexnum+1,str_id.indexOf("[")-indexnum-1);
        str_id.remove(0,1);
        str_id.prepend("A");
    }else{
        if(str_id.left(1)==" "){
           strtemp = str_id.mid(indexnum,str_id.indexOf("[")-indexnum);
           if(strtemp.left(1)=="P"||strtemp.left(1)=="L"||strtemp.left(1)=="R"){
               strkey = strtemp.mid(1,strtemp.length()-1);
               str_id.remove(3,1);
               str_id.insert(3,"A");
           }else{
               strkey = strtemp;
               str_id.insert(3,"A");
           }
        }else{
           strkey = str_id.left(str_id.indexOf("["));
           str_id.prepend("A");
        }

    }

    if(m_map_id.contains(strkey.toInt())){
        QMap< int, int >::iterator it_id = m_map_id.find(strkey.toInt());
        strvalue = QString::number(it_id.value());
        str_id.remove(indexnum+1,str_id.indexOf("[")-indexnum-1);
        str_id.insert(indexnum+1,strvalue);
    }else{
        if( it_preset != m_map_preset_general_ana.end() ){
            for( it_ana = it_preset.value().begin(); it_ana != it_preset.value().end(); it_ana++ ){
                //set_anaitem_to_stringlist( it_ana.value(), str_alarm_list );
                if(strkey==it_ana.value().str_name){
                    strvalue = QString::number(it_ana.value().n_id_ana);
                    str_id.replace(strkey,strvalue);
                    break;
                }
            }
        }
    }*/

}

void WndMatrixCnd::get_matrix_color_name_by_ana_id( int n_ana_id, QString &str_out_color_name )
{
    //ui->id_pre_frame_ir->get_matrix_color_name_by_ana_id( n_ana_id, out_color_name);
}


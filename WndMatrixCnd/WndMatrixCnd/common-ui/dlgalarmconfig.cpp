#include "dlgalarmconfig.h"
#include "ui_dlgalarmconfig.h"
#include "dlgalarmcondition.h"
#include  <QFileDialog>
#include  <QStandardItemModel>
#include <QPainter>
#include "datamode.h"

DlgAlarmConfig::DlgAlarmConfig(QString s_sn,int n_type,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAlarmConfig),
    ms_sn(s_sn),
    mn_type(n_type),
    m_pixmap_background( ":image/image/caption_background.bmp" )
{

    ui->setupUi(this);

    connect(ui->btnAdd,SIGNAL(clicked()),this,SLOT(slot_on_add()));
    connect(ui->btnDel,SIGNAL(clicked()),this,SLOT(slot_on_del()));
    connect(ui->btnClear,SIGNAL(clicked()),this,SLOT(slot_on_clear()));
    connect(ui->btnDirectory,SIGNAL(clicked()),this,SLOT(slot_on_dir()));
    connect(ui->radioCapture,SIGNAL(clicked()),this,SLOT(slot_on_capture()));
    connect(ui->radioNoCapture,SIGNAL(clicked()),this,SLOT(slot_on_notcapture()));
    connect(ui->checkBoxBeep,SIGNAL(clicked()),this,SLOT(slot_on_beep()));
    connect(ui->checkBoxMsg,SIGNAL(clicked()),this,SLOT(slot_on_msg()));

    if( s_sn != "")
      update_data(s_sn);
}

DlgAlarmConfig::~DlgAlarmConfig()
{
    delete ui;
}
void DlgAlarmConfig::update_data(QString& s_sn)
{
    ms_sn = s_sn;
    ui->radioCapture->setChecked(true);
    int n_cature = CAlarmCfg::Instance()->get_capture(ms_sn,mn_type);
    if( n_cature == 0)
    {
        ui->radioNoCapture->setChecked(true);
    }

    int n_msg=0;
    QString s_msg="";
    CAlarmCfg::Instance()->get_message(ms_sn,mn_type,n_msg,s_msg);
    ui->checkBoxMsg->setChecked(false);
    if( n_msg != 0) ui->checkBoxMsg->setChecked(true);

    int n_beep=0;
    n_beep=CAlarmCfg::Instance()->get_beep(ms_sn,mn_type);
    ui->checkBoxBeep->setChecked(false);
    if( n_beep != 0) ui->checkBoxBeep->setChecked(true);


    QString s_text ;
    CAlarmCfg::Instance()->get_capture_path(ms_sn,mn_type,s_text);
    ui->lineEdit->setText( s_text);

    QStandardItemModel *model = new QStandardItemModel();
    ui->listView->setModel( model );
    vector<AlarmGroup>  vgroups;
    vgroups.clear();
    CAlarmCfg::Instance()->get_group(ms_sn,mn_type,vgroups);
    for(int i=0 ; i<vgroups.size(); i++)
    {
        AlarmGroup *po_group = new AlarmGroup;
        *po_group = vgroups[i];

        insert_group(po_group);

    }
}

void DlgAlarmConfig::slot_on_add()
{
   QString s_sn = ms_sn;
   if( mn_type==0 ) {
       s_sn +="_real";
   } else {
       s_sn +="_record";
   }

   QStringList s_list;

   QMap< int, AnaInfo > map_ana_info;
   CDataMode::Instance()->get_ana_info( s_sn, map_ana_info );

   QMap< int, AnaInfo >::iterator iter = map_ana_info.begin();
   while( iter !=  map_ana_info.end() ){
       QString s_info = iter.value().type_name;
       s_info+="_%1";
       s_info = s_info.arg( iter.value().n_sign_id);
       s_list.append(s_info);
       ++iter;
   }


    DlgAlarmCondition dlg(this);
    dlg.set_ana_info(s_list);
    if( QDialog::Accepted == dlg.exec() )
    {
        AlarmGroup *po_group = new AlarmGroup;
        *po_group = dlg.m_alarm_group;
        insert_group(po_group);
        save_group();
    }
}

void DlgAlarmConfig::slot_on_del()
{
    QStandardItemModel *model=(QStandardItemModel *) ui->listView->model();
    for( int i=0; i< model->rowCount(); i++)
    {
        QStandardItem* item=(QStandardItem*) model->item(i,0);
        if(Qt::Checked==item->data(Qt::CheckStateRole).toInt() )
        {
            AlarmGroup  *po_group= (AlarmGroup* )item->data(Qt::UserRole).toInt();
            model->removeRow(i);
            i=-1;
            delete po_group;
        }
    }

    save_group();

}
void DlgAlarmConfig::slot_on_beep()
{
    int nCheck = 0;
    if( Qt::Checked == ui->checkBoxBeep->checkState() ) nCheck=1;

    CAlarmCfg::Instance()->set_beep(ms_sn,mn_type,nCheck);
}

void DlgAlarmConfig::slot_on_msg()
{
    int nCheck = 0;
    if( Qt::Checked == ui->checkBoxMsg->checkState() ) nCheck=1;

    QString s_msg="";
    CAlarmCfg::Instance()->set_message(ms_sn,mn_type,nCheck,s_msg);
}
void DlgAlarmConfig::insert_group(AlarmGroup *po_group)
{
 /*   QStandardItemModel *model=(QStandardItemModel *) ui->listView->model();

    QStandardItem *p_item = new QStandardItem();
    p_item->setCheckable( true );
    p_item->setCheckState( Qt::Unchecked );
    QString s_text="and{";
    if( po_group->mn_AndOper ==0 ) s_text="or{";

    for(int m=0; m<po_group->mv_conds.size();m++)
    {
        s_text+="[";
        s_text+=po_group->mv_conds[m].ms_atype;

        QString cOper[3]={"-","","+"};

        int n_index = po_group->mv_conds[m].mn_oper+1;
        if( n_index<0 ) n_index=0;
        if( n_index>2 ) n_index=2;
        s_text+=cOper[n_index];
        s_text+=po_group->mv_conds[m].ms_btype;

        QString c_compare[5]={">","<","=",">=","<="};

        n_index = po_group->mv_conds[m].mn_compare;
        if( n_index<0 ) n_index=0;
        if( n_index>4 ) n_index=4;
        s_text+=c_compare[n_index];
        s_text+="%1";
        s_text = s_text.arg(po_group->mv_conds[m].mf_value);

        s_text+="]";
    }

    s_text+="}";
    p_item->setText(s_text);
    p_item->setData((int)po_group,Qt::UserRole);
    model->setItem( model->rowCount(), p_item );*/
}

void DlgAlarmConfig::save_group()
{
    vector<AlarmGroup>  vgroups;
    vgroups.clear();

    QStandardItemModel *model=(QStandardItemModel *) ui->listView->model();
    for( int i=0; i< model->rowCount(); i++)
    {
        QStandardItem* item=(QStandardItem*) model->item(i,0);
        AlarmGroup  *po_group= (AlarmGroup* )item->data(Qt::UserRole).toInt();
        vgroups.push_back(*po_group);
    }

    CAlarmCfg::Instance()->set_group(ms_sn,mn_type,vgroups);
}

void DlgAlarmConfig::slot_on_clear()
{
     QStandardItemModel *model=(QStandardItemModel *) ui->listView->model();
     for( int i=0; i< model->rowCount(); i++)
     {
         QStandardItem* item=(QStandardItem*) model->item(i,0);
         AlarmGroup  *po_group= (AlarmGroup* )item->data(Qt::UserRole).toInt();
         delete po_group;
     }

     model->clear();

     save_group();

}

void DlgAlarmConfig::slot_on_capture()
{
   CAlarmCfg::Instance()->set_capture(ms_sn,mn_type,1);
}

void DlgAlarmConfig::slot_on_notcapture()
{
   CAlarmCfg::Instance()->set_capture(ms_sn,mn_type,0);
}

void DlgAlarmConfig::slot_on_dir()
{
    QString sDir = ui->lineEdit->text();
    QString s_dir = QFileDialog::getExistingDirectory(this,tr("Open Directory"),sDir);
    if( s_dir.length() > 0 )
    {
       ui->lineEdit->setText(s_dir);
       CAlarmCfg::Instance()->set_capture_path(ms_sn,mn_type,s_dir);
    }
}
void DlgAlarmConfig::paintEvent( QPaintEvent *event )
{
   QPainter draw(this);
   draw.drawPixmap( 0, 0, width(), height(), m_pixmap_background );
   draw.end();
}
void DlgAlarmConfig::hide_ok_cancel()
{
    ui->buttonBox->hide();
}

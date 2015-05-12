#include "dlgalarmcondition.h"
#include "ui_dlgalarmcondition.h"
#include <QMessageBox>
#include <QPainter>

DlgAlarmCondition::DlgAlarmCondition(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAlarmCondition)
    ,m_pixmap_background( ":image/image/caption_background.bmp" )
    ,mb_matrix_ana( false )
{
    ui->setupUi(this);

    setWindowTitle(tr("分析报警条件设置"));

    ui->radioOr->setChecked(true);
    ui->id_pre_radio_general_ana->setChecked( true );
    DlgAlarmCondition::setMaximumSize(444,316);
    DlgAlarmCondition::setMinimumSize(444,316);

    connect(ui->id_pre_radio_general_ana, SIGNAL(clicked()), this, SLOT(slot_radio_btn()));
    connect(ui->id_pre_radio_array_ana, SIGNAL(clicked()), this, SLOT(slot_radio_btn()));

    QString s_value="0.0";
    QDoubleValidator *p_validator = new QDoubleValidator(-100, 100,3, ui->lineExp);
    p_validator->setRange(-100, 100,3);
    p_validator->setNotation(QDoubleValidator::StandardNotation);
    ui->lineExp->setValidator(p_validator);
    ui->lineExp->setText(s_value);

    p_validator = new QDoubleValidator(-100, 100,3, ui->lineExp);
    p_validator->setRange(-100, 100,3);
    p_validator->setNotation(QDoubleValidator::StandardNotation);
    ui->lineExp_2->setValidator(p_validator);
    ui->lineExp_2->setText(s_value);

    p_validator = new QDoubleValidator(-100, 100,3, ui->lineExp);
    p_validator->setRange(-100, 100,3);
    p_validator->setNotation(QDoubleValidator::StandardNotation);
    ui->lineExp_3->setValidator(p_validator);
    ui->lineExp_3->setText(s_value);

    p_validator = new QDoubleValidator(-100, 100,3, ui->lineExp);
    p_validator->setRange(-100, 100,3);
    p_validator->setNotation(QDoubleValidator::StandardNotation);
    ui->lineExp_4->setValidator(p_validator);
    ui->lineExp_4->setText(s_value);

    ui->combo_aType->setEditable(false);
    ui->combo_aType->addItem(QString(""));
    ui->combo_aType->setItemData(0,-1);
    ui->combo_aType->setCurrentIndex(0);

    ui->combo_aType_2->setEditable(false);
    ui->combo_aType_2->addItem(QString(""));
    ui->combo_aType_2->setItemData(0,-1);
    ui->combo_aType_2->setCurrentIndex(0);

    ui->combo_aType_3->setEditable(false);
    ui->combo_aType_3->addItem(QString(""));
    ui->combo_aType_3->setItemData(0,-1);
    ui->combo_aType_3->setCurrentIndex(0);

    ui->combo_aType_4->setEditable(false);
    ui->combo_aType_4->addItem(QString(""));
    ui->combo_aType_4->setItemData(0,-1);
    ui->combo_aType_4->setCurrentIndex(0);

    ui->combo_bType->setEditable(false);
    ui->combo_bType->addItem(QString(""));
    ui->combo_bType->setItemData(0,-1);
    ui->combo_bType->setCurrentIndex(0);
    ui->combo_bType->setEnabled(false);

    ui->combo_bType_2->setEditable(false);
    ui->combo_bType_2->addItem(QString(""));
    ui->combo_bType_2->setItemData(0,-1);
    ui->combo_bType_2->setCurrentIndex(0);
    ui->combo_bType_2->setEnabled(false);

    ui->combo_bType_3->setEditable(false);
    ui->combo_bType_3->addItem(QString(""));
    ui->combo_bType_3->setItemData(0,-1);
    ui->combo_bType_3->setCurrentIndex(0);
    ui->combo_bType_3->setEnabled(false);

    ui->combo_bType_4->setEditable(false);
    ui->combo_bType_4->addItem(QString(""));
    ui->combo_bType_4->setItemData(0,-1);
    ui->combo_bType_4->setCurrentIndex(0);
    ui->combo_bType_4->setEnabled(false);

    ui->comboOper->addItem(QString(""));
    ui->comboOper->setItemData(0,0);
    ui->comboOper->addItem(QString("-"));
    ui->comboOper->setItemData(1,-1);
    ui->comboOper->addItem(QString("+"));
    ui->comboOper->setItemData(2,1);
    ui->comboOper->setEditable(false);
    ui->comboOper->setCurrentIndex(0);

    ui->comboOper_2->addItem(QString(""));
    ui->comboOper_2->setItemData(0,0);
    ui->comboOper_2->addItem(QString("-"));
    ui->comboOper_2->setItemData(1,-1);
    ui->comboOper_2->addItem(QString("+"));
    ui->comboOper_2->setItemData(2,1);
    ui->comboOper_2->setEditable(false);
    ui->comboOper_2->setCurrentIndex(0);

    ui->comboOper_3->addItem(QString(""));
    ui->comboOper_3->setItemData(0,0);
    ui->comboOper_3->addItem(QString("-"));
    ui->comboOper_3->setItemData(1,-1);
    ui->comboOper_3->addItem(QString("+"));
    ui->comboOper_3->setItemData(2,1);
    ui->comboOper_3->setEditable(false);
    ui->comboOper_3->setCurrentIndex(0);

    ui->comboOper_4->addItem(QString(""));
    ui->comboOper_4->setItemData(0,0);
    ui->comboOper_4->addItem(QString("-"));
    ui->comboOper_4->setItemData(1,-1);
    ui->comboOper_4->addItem(QString("+"));
    ui->comboOper_4->setItemData(2,1);
    ui->comboOper_4->setEditable(false);
    ui->comboOper_4->setCurrentIndex(0);

    ui->comboCompare->addItem(QString(">"));
    ui->comboCompare->setItemData(0,0);
    ui->comboCompare->addItem(QString("<"));
    ui->comboCompare->setItemData(1,1);
    ui->comboCompare->addItem(QString("="));
    ui->comboCompare->setItemData(2,2);
    ui->comboCompare->addItem(QString(">="));
    ui->comboCompare->setItemData(3,3);
    ui->comboCompare->addItem(QString("<="));
    ui->comboCompare->setItemData(4,4);
    ui->comboCompare->setEditable(false);
    ui->comboCompare->setCurrentIndex(0);

    ui->comboCompare_2->addItem(QString(">"));
    ui->comboCompare_2->setItemData(0,0);
    ui->comboCompare_2->addItem(QString("<"));
    ui->comboCompare_2->setItemData(1,1);
    ui->comboCompare_2->addItem(QString("="));
    ui->comboCompare->setItemData(2,2);
    ui->comboCompare_2->addItem(QString(">="));
    ui->comboCompare_2->setItemData(3,3);
    ui->comboCompare_2->addItem(QString("<="));
    ui->comboCompare_2->setItemData(4,4);
    ui->comboCompare_2->setEditable(false);
    ui->comboCompare_2->setCurrentIndex(0);

    ui->comboCompare_3->addItem(QString(">"));
    ui->comboCompare_3->setItemData(0,0);
    ui->comboCompare_3->addItem(QString("<"));
    ui->comboCompare_3->setItemData(1,1);
    ui->comboCompare_3->addItem(QString("="));
    ui->comboCompare_3->setItemData(2,2);
    ui->comboCompare_3->addItem(QString(">="));
    ui->comboCompare_3->setItemData(3,3);
    ui->comboCompare_3->addItem(QString("<="));
    ui->comboCompare_3->setItemData(4,4);
    ui->comboCompare_3->setEditable(false);
    ui->comboCompare_3->setCurrentIndex(0);

    ui->comboCompare_4->addItem(QString(">"));
    ui->comboCompare_4->setItemData(0,0);
    ui->comboCompare_4->addItem(QString("<"));
    ui->comboCompare_4->setItemData(1,1);
    ui->comboCompare_4->addItem(QString("="));
    ui->comboCompare_4->setItemData(2,2);
    ui->comboCompare_4->addItem(QString(">="));
    ui->comboCompare_4->setItemData(3,3);
    ui->comboCompare_4->addItem(QString("<="));
    ui->comboCompare_4->setItemData(4,4);
    ui->comboCompare_4->setEditable(false);
    ui->comboCompare_4->setCurrentIndex(0);

    connect(ui->btnClear,SIGNAL(clicked()),this,SLOT(slot_on_clear()));
    connect(ui->btnClear_2,SIGNAL(clicked()),this,SLOT(slot_on_clear()));
    connect(ui->btnClear_3,SIGNAL(clicked()),this,SLOT(slot_on_clear()));
    connect(ui->btnClear_4,SIGNAL(clicked()),this,SLOT(slot_on_clear()));

    connect(ui->pushButtonOk,SIGNAL(clicked()),this,SLOT(slot_on_ok()));
    connect(ui->pushButtonCancel,SIGNAL(clicked()),this,SLOT(reject()));

    connect(ui->comboOper,SIGNAL(activated(QString)),this,SLOT(slot_activated(QString )));
    connect(ui->comboOper_2,SIGNAL(activated(QString)),this,SLOT(slot_activated(QString )));
    connect(ui->comboOper_3,SIGNAL(activated(QString)),this,SLOT(slot_activated(QString )));
    connect(ui->comboOper_4,SIGNAL(activated(QString)),this,SLOT(slot_activated(QString )));


}
 void DlgAlarmCondition::set_ana_info( QStringList& s_list)
 {
     ms_list = s_list;
     qDebug()<<"list:"<<ms_list<<"size="<<ms_list.size();
     clear_combo_info();
     ui->combo_aType->addItem("");
     ui->combo_bType->addItem("");
     ui->combo_aType_2->addItem("");
     ui->combo_bType_2->addItem("");
     ui->combo_aType_3->addItem("");
     ui->combo_bType_3->addItem("");
     ui->combo_aType_4->addItem("");
     ui->combo_bType_4->addItem("");

     for(int i=0; i<ms_list.size(); i++){
         QString s_text = ms_list.at(i);
         ui->combo_aType->addItem(s_text);
         ui->combo_bType->addItem(s_text);
         ui->combo_aType_2->addItem(s_text);
         ui->combo_bType_2->addItem(s_text);
         ui->combo_aType_3->addItem(s_text);
         ui->combo_bType_3->addItem(s_text);
         ui->combo_aType_4->addItem(s_text);
         ui->combo_bType_4->addItem(s_text);
     }

     ui->comboOper->setCurrentIndex(0);
     ui->comboOper_2->setCurrentIndex(0);
     ui->comboOper_3->setCurrentIndex(0);
     ui->comboOper_4->setCurrentIndex(0);
     ui->combo_bType->setEnabled( false );
     ui->combo_bType_2->setEnabled( false );
     ui->combo_bType_3->setEnabled( false );
     ui->combo_bType_4->setEnabled( false );
 }

 void DlgAlarmCondition::set_matrix_bool( bool b_matrix )
 {
     mb_matrix_ana = b_matrix;
 }

 void DlgAlarmCondition::clear_combo_info()
 {
     QString s_value = "0.0";
     ui->combo_aType->clear();
     ui->combo_bType->clear();
     ui->combo_aType_2->clear();
     ui->combo_bType_2->clear();
     ui->combo_aType_3->clear();
     ui->combo_bType_3->clear();
     ui->combo_aType_4->clear();
     ui->combo_bType_4->clear();


     ui->lineExp->setText(s_value);
     ui->lineExp_2->setText(s_value);
     ui->lineExp_3->setText(s_value);
     ui->lineExp_4->setText(s_value);

 }


void DlgAlarmCondition::set_default_ana_info( AlarmGroup *p_alarm_group )
{
     int index = -1;
     QString str_value = "%1";
     bool b_value = false;
     vector<AlarmCondition>::iterator it_cond = p_alarm_group->mv_conds.begin();
     if( it_cond != p_alarm_group->mv_conds.end() ){
         index = ui->combo_aType->findText( it_cond->ms_atype );
         ui->combo_aType->setCurrentIndex(index);
         index = ui->comboOper->findText( it_cond->mn_oper );
         ui->comboOper->setCurrentIndex(index);
         index = ui->combo_bType->findText( it_cond->ms_btype );
         ui->combo_bType->setCurrentIndex(index);    
         ui->combo_bType->setEnabled( b_value ? index > 0 : index < 1 );
         index = ui->comboCompare->findText( it_cond->mn_compare );
         ui->comboCompare->setCurrentIndex(index);
         str_value = str_value.arg( it_cond->mf_value );
         //str_value = it_cond->mf_value;
         ui->lineExp->setText( str_value );
     } else {
         return;
     }
     it_cond++;
     if( it_cond != p_alarm_group->mv_conds.end() ){
         index = ui->combo_aType_2->findText( it_cond->ms_atype );
         ui->combo_aType_2->setCurrentIndex(index);
         index = ui->comboOper_2->findText( it_cond->mn_oper );
         ui->comboOper_2->setCurrentIndex(index);
         index = ui->combo_bType_2->findText( it_cond->ms_btype );
         ui->combo_bType_2->setCurrentIndex(index);
         ui->combo_bType_2->setEnabled( b_value ? index > 0 : index < 1 );
         index = ui->comboCompare_2->findText( it_cond->mn_compare );
         ui->comboCompare_2->setCurrentIndex(index);
         str_value = str_value.arg( it_cond->mf_value );
         ui->lineExp_2->setText( str_value );
     } else {
         return;
     }
     it_cond++;
     if( it_cond != p_alarm_group->mv_conds.end() ){
         index = ui->combo_aType_3->findText( it_cond->ms_atype );
         ui->combo_aType_3->setCurrentIndex(index);
         index = ui->comboOper_3->findText( it_cond->mn_oper );
         ui->comboOper_3->setCurrentIndex(index);
         index = ui->combo_bType_3->findText( it_cond->ms_btype );
         ui->combo_bType_3->setCurrentIndex(index);
         ui->combo_bType_3->setEnabled( b_value ? index > 0 : index < 1 );
         index = ui->comboCompare_3->findText( it_cond->mn_compare );
         ui->comboCompare_3->setCurrentIndex(index);
         str_value = str_value.arg( it_cond->mf_value );
         ui->lineExp_2->setText( str_value );
     } else {
         return;
     }
     it_cond++;
     if( it_cond != p_alarm_group->mv_conds.end() ){
         index = ui->combo_aType_4->findText( it_cond->ms_atype );
         ui->combo_aType_4->setCurrentIndex(index);
         index = ui->comboOper_4->findText( it_cond->mn_oper );
         ui->comboOper_4->setCurrentIndex(index);
         index = ui->combo_bType_4->findText( it_cond->ms_btype );
         ui->combo_bType_4->setCurrentIndex(index);
         ui->combo_bType_4->setEnabled( b_value ? index > 0 : index < 1 );
         index = ui->comboCompare_4->findText( it_cond->mn_compare );
         ui->comboCompare_4->setCurrentIndex(index);
         str_value = str_value.arg( it_cond->mf_value );
         ui->lineExp_2->setText( str_value );
     } else {
         return;
     }
     it_cond++;
}


void DlgAlarmCondition::slot_activated(QString str_oper)
{
    QComboBox* po_oper = (QComboBox* ) sender();

    if( po_oper== NULL) return ;

    bool b_enable=true;
    if( po_oper->currentIndex() == 0 ) b_enable=false;

    if( po_oper == ui->comboOper ){
        ui->combo_bType->setEnabled(b_enable);
    }
    else if( po_oper == ui->comboOper_2 ){
        ui->combo_bType_2->setEnabled(b_enable);
    }
    else if( po_oper == ui->comboOper_3 ){
        ui->combo_bType_3->setEnabled(b_enable);
    }
    else if( po_oper == ui->comboOper_4 ){
        ui->combo_bType_4->setEnabled(b_enable);
    }

}

DlgAlarmCondition::~DlgAlarmCondition()
{
    delete ui;
}
 void DlgAlarmCondition::slot_on_clear()
 {
     QPushButton* po_cliecked = (QPushButton* ) sender();

     if( po_cliecked== NULL) return ;

     if( po_cliecked == ui->btnClear){
        clear_row_info( ui->combo_aType, ui->comboOper, ui->combo_bType, ui->comboCompare, ui->lineExp );
     }
     else if(po_cliecked == ui->btnClear_2){
        clear_row_info( ui->combo_aType_2, ui->comboOper_2, ui->combo_bType_2, ui->comboCompare_2, ui->lineExp_2 );
     }
     else if(po_cliecked == ui->btnClear_3){
        clear_row_info( ui->combo_aType_3, ui->comboOper_3, ui->combo_bType_3, ui->comboCompare_3, ui->lineExp_3 );
     }
     else if(po_cliecked == ui->btnClear_4){
        clear_row_info( ui->combo_aType_4, ui->comboOper_4, ui->combo_bType_4, ui->comboCompare_4, ui->lineExp_4 );
     }

 }


  void DlgAlarmCondition::clear_row_info( QObject *p_combo_atype, QObject *p_combo_oper,
                                          QObject *p_combo_btype, QObject *p_combo_compare, QObject *p_line_value )
  {
      QString s_value="0.0";
      ((QComboBox*)p_combo_atype)->setCurrentIndex(0);
      ((QComboBox*)p_combo_oper)->setCurrentIndex(0);
      ((QComboBox*)p_combo_btype)->setCurrentIndex(0);
      ((QComboBox*)p_combo_compare)->setCurrentIndex(0);
      ((QLineEdit*)p_line_value)->setText( s_value );
  }


 void DlgAlarmCondition::slot_on_ok()
 {
     m_alarm_group.mv_conds.clear();

     m_alarm_group.mn_AndOper = 1;
     if( ui->radioOr->isChecked() ) m_alarm_group.mn_AndOper = 0;
     init_alarm_group_value( ui->combo_aType, ui->comboOper, ui->combo_bType, ui->comboCompare, ui->lineExp );
     init_alarm_group_value( ui->combo_aType_2, ui->comboOper_2, ui->combo_bType_2, ui->comboCompare_2, ui->lineExp_2 );
     init_alarm_group_value( ui->combo_aType_3, ui->comboOper_3, ui->combo_bType_3, ui->comboCompare_3, ui->lineExp_3 );
     init_alarm_group_value( ui->combo_aType_4, ui->comboOper_4, ui->combo_bType_4, ui->comboCompare_4, ui->lineExp_4 );

     if( m_alarm_group.mv_conds.size() == 0 ){
         QMessageBox::information(this, tr("infomation"), tr("请选择报警条件项!"));
         return ;
     }
     this->accept();
 }


void DlgAlarmCondition::init_alarm_group_value( QObject *p_combo_atype, QObject *p_combo_oper,
                                                QObject *p_combo_btype, QObject *p_combo_compare, QObject *p_line_value )
{
    QString s_atype;
    QString s_btype;
    AlarmCondition alarm_condition;
    s_atype = ((QComboBox*)p_combo_atype)->currentText();
    s_btype = ((QComboBox*)p_combo_btype)->currentText();
    if( s_atype != "" ){
       alarm_condition.ms_atype = s_atype;
       if( s_btype != "" ){
           alarm_condition.ms_btype = s_btype;
           alarm_condition.mn_oper = ((QComboBox*)p_combo_oper)->currentText();
       }
       alarm_condition.mn_compare = ((QComboBox*)p_combo_compare)->currentText();
       alarm_condition.mf_value = ((QLineEdit*)p_line_value)->text().toFloat();
       m_alarm_group.mv_conds.push_back(alarm_condition);
    }
}

 void DlgAlarmCondition::paintEvent( QPaintEvent *event )
 {
    QPainter draw( this );
    draw.drawPixmap( 0, 0, width(), height(), m_pixmap_background );
    draw.end();
 }


 void DlgAlarmCondition::slot_radio_btn()
 {
     QObject *p_sender = QObject::sender();//在槽内获取信号的发送者
     if( p_sender == ui->id_pre_radio_general_ana ){
         mb_matrix_ana = false;
         emit sig_radio_btn( 1 );  //普通分析
     } else if( p_sender == ui->id_pre_radio_array_ana ){
         mb_matrix_ana = true;
         emit sig_radio_btn( 2 );   //阵列分析
     }
 }

 void DlgAlarmCondition::set_check_general(bool b_checked)
 {
     ui->id_pre_radio_general_ana->setChecked( b_checked );
     mb_matrix_ana = false;
 }

  void DlgAlarmCondition::set_check_general_enable( bool b_enabled )
  {
      ui->id_pre_radio_general_ana->setEnabled( b_enabled );
  }

 void DlgAlarmCondition::set_check_array(bool b_checked)
 {
     ui->id_pre_radio_array_ana->setChecked( b_checked );
     mb_matrix_ana = true;
 }

 void DlgAlarmCondition::set_check_array_enable( bool b_enabled )
 {
     ui->id_pre_radio_array_ana->setEnabled( b_enabled );
 }

 void DlgAlarmCondition::set_combo_oper_enable( bool b_enabled )
 {
     ui->comboOper->setEnabled(b_enabled);
     ui->comboOper_2->setEnabled(b_enabled);
     ui->comboOper_3->setEnabled(b_enabled);
     ui->comboOper_4->setEnabled(b_enabled);
 }

  void DlgAlarmCondition::set_matrix_combo_enable( bool b_enabled )
  {
      ui->combo_bType->setEnabled( b_enabled );
      ui->combo_aType_2->setEnabled( b_enabled );
      ui->combo_bType_2->setEnabled( b_enabled );
      ui->combo_aType_3->setEnabled( b_enabled );
      ui->combo_bType_3->setEnabled( b_enabled );
      ui->combo_aType_4->setEnabled( b_enabled );
      ui->combo_bType_4->setEnabled( b_enabled );

      ui->comboCompare_2->setEnabled( b_enabled );
      ui->comboCompare_3->setEnabled( b_enabled );
      ui->comboCompare_4->setEnabled( b_enabled );
      ui->lineExp_2->setEnabled( b_enabled );
      ui->lineExp_3->setEnabled( b_enabled );
      ui->lineExp_4->setEnabled( b_enabled );

      ui->comboOper->setEnabled( b_enabled );
      ui->comboOper_2->setEnabled( b_enabled );
      ui->comboOper_3->setEnabled( b_enabled );
      ui->comboOper_4->setEnabled( b_enabled );

      ui->btnClear_2->setEnabled( b_enabled );
      ui->btnClear_3->setEnabled( b_enabled );
      ui->btnClear_4->setEnabled( b_enabled );
  }

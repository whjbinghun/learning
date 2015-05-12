#include "alarmcfg.h"
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QVariant>
#include "datamode.h"

  CAlarmCfg* CAlarmCfg::m_pInstance=0;

  CAlarmCfg* CAlarmCfg::Instance()
  {
      if( 0 == m_pInstance )
      {
          m_pInstance = new CAlarmCfg();
      }

      return m_pInstance;
  }

  void CAlarmCfg::Uninstance()
  {
      if( 0 != m_pInstance )
      {
          delete m_pInstance;
          m_pInstance=0;
      }
  }


CAlarmCfg::CAlarmCfg():m_rwLock(QReadWriteLock::Recursive)
{
    QWriteLocker lock(&m_rwLock);

    QString sDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    qDebug()<<sDir;

    sDir+="/meisheng_client/";

    QDir temp;
    if( !temp.exists(sDir) )
    {
       temp.mkdir(sDir);
    }
    sDir+= "alarm_config.ini";

    mp_setting = new QSettings(sDir,QSettings::IniFormat);

}
CAlarmCfg::~CAlarmCfg()
{
    QWriteLocker lock(&m_rwLock);
    if( mp_setting != NULL )
    {
        delete mp_setting;

        mp_setting=NULL;
    }
}


int  CAlarmCfg::get_capture_path(QString& sSN,int nType,QString& str_path)
{

     QString sKey =sSN+"_";
     QString sType="real";

     if( nType!=0 ) sType="record";
     sKey+=sType;

     sKey+="/capturepath";

     QReadLocker lock(&m_rwLock);
     str_path = mp_setting->value(sKey).toString();

     return 0;
}

int  CAlarmCfg::set_capture_path(QString& sSN,int nType,QString& str_path)
{

    QString sKey =sSN+"_";
    QString sType="real";

    if( nType!=0 ) sType="record";
    sKey+=sType;

    sKey+="/capturepath";

     QWriteLocker lock(&m_rwLock);
    mp_setting->setValue(sKey,str_path);

    return 0;
}

int  CAlarmCfg::get_capture(QString& sSN,int nType)
{

    QString sKey =sSN+"_";
    QString sType="real";
    bool bRet=false;

    if( nType!=0 ) sType="record";
    sKey+=sType;

    sKey+="/capture";

     QReadLocker lock(&m_rwLock);
    int nCapture = mp_setting->value(sKey).toInt(&bRet);

    if( !bRet ) return 0;

    return nCapture;
}

int  CAlarmCfg::set_capture(QString& sSN,int nType,int nFlag)
{

    QString sKey =sSN+"_";
    QString sType="real";

    if( nType!=0 ) sType="record";
    sKey+=sType;

    sKey+="/capture";

    QWriteLocker lock(&m_rwLock);
    mp_setting->setValue(sKey,nFlag);

    return 0;
}

int  CAlarmCfg::get_beep(QString& sSN,int nType)
{

    QString sKey =sSN+"_";
    QString sType="real";
    bool bRet=false;

    if( nType!=0 ) sType="record";
    sKey+=sType;

    sKey+="/beep";

     QReadLocker lock(&m_rwLock);
    int nBeep = mp_setting->value(sKey).toInt(&bRet);

    if( !bRet ) return 0;

    return nBeep;
}

int  CAlarmCfg::set_beep(QString& sSN,int nType,int nFlag)
{

    QString sKey =sSN+"_";
    QString sType="real";

    if( nType!=0 ) sType="record";
    sKey+=sType;

    sKey+="/beep";

    QWriteLocker lock(&m_rwLock);
    mp_setting->setValue(sKey,nFlag);
    return 0;
}

int  CAlarmCfg::get_message(QString& sSN,int nType,int& nFlag,QString& sMsg)
{

    QString sKey =sSN+"_";
    QString sType="real";
    bool bRet=false;

    if( nType!=0 ) sType="record";
    sKey+=sType;

    sKey+="/msg";

    QReadLocker lock(&m_rwLock);
    nFlag = mp_setting->value(sKey).toInt(&bRet);
    if( !bRet ) nFlag=0;

    sKey+="conten";
    sMsg = mp_setting->value(sKey).toString();

    return 0;
}

int  CAlarmCfg::set_message(QString& sSN,int nType,int nFlag,QString& sMsg)
{
    QString sKey =sSN+"_";
    QString sType="real";

    if( nType!=0 ) sType="record";
    sKey+=sType;

    sKey+="/msg";

    QWriteLocker lock(&m_rwLock);

    mp_setting->setValue(sKey,nFlag);

    sKey+="conten";
    mp_setting->setValue(sKey,sMsg);

    return 0;
}

int  CAlarmCfg::set_group(QString& sSN,int nType, vector<AlarmGroup>& vgroups)
{
/*    QString sKey =sSN+"_";
    QString sType="real";

    if( nType!=0 ) sType="record";
    sKey+=sType;
    sKey+="/";

    QString sGroupKey=sKey+"groupcount";

    QWriteLocker lock(&m_rwLock);

    mp_setting->setValue(sGroupKey,vgroups.size());

    for(int i=0; i<vgroups.size();i++)
    {
        sGroupKey=sKey+"oper%1";
        sGroupKey=sGroupKey.arg(i);
        mp_setting->setValue(sGroupKey,vgroups[i].mn_AndOper);

        sGroupKey=sKey+"group%1";
        sGroupKey=sGroupKey.arg(i);
        mp_setting->setValue(sGroupKey,vgroups[i].mv_conds.size());

        for(int m=0; m<vgroups[i].mv_conds.size(); m++)
        {
            sGroupKey=sKey+"group%1/atype%2";
            sGroupKey=sGroupKey.arg(i).arg(m);
            mp_setting->setValue(sGroupKey,vgroups[i].mv_conds[m].ms_atype);

            sGroupKey=sKey+"group%1/btype%2";
            sGroupKey=sGroupKey.arg(i).arg(m);
            mp_setting->setValue(sGroupKey,vgroups[i].mv_conds[m].ms_btype);

            sGroupKey=sKey+"group%1/oper%2";
            sGroupKey=sGroupKey.arg(i).arg(m);
            mp_setting->setValue(sGroupKey,vgroups[i].mv_conds[m].mn_oper);

            sGroupKey=sKey+"group%1/compare%2";
            sGroupKey=sGroupKey.arg(i).arg(m);
            mp_setting->setValue(sGroupKey,vgroups[i].mv_conds[m].mn_compare);

            sGroupKey=sKey+"group%1/value%2";
            sGroupKey=sGroupKey.arg(i).arg(m);

            QString s_value="%1";
            s_value=s_value.arg(vgroups[i].mv_conds[m].mf_value);
            mp_setting->setValue(sGroupKey,s_value);
        }
    }*/

    return 0;
}
int  CAlarmCfg::get_group(QString& sSN,int nType, vector<AlarmGroup>& vgroups)
{

    vgroups.clear();


/*    QString sKey =sSN+"_";
    QString sType="real";

    if( nType!=0 ) sType="record";
    sKey+=sType;
    sKey+="/";

    bool    bOk=false;
    int     n_group_count=0;
    QString sGroupKey=sKey+"groupcount";

     QReadLocker lock(&m_rwLock);
    n_group_count = mp_setting->value(sGroupKey,0).toInt(&bOk);
    if( !bOk ) return -1;

    for(int i=0; i<n_group_count;i++)
    {
        AlarmGroup group;
        sGroupKey=sKey+"oper%1";
        sGroupKey=sGroupKey.arg(i);

        group.mn_AndOper=0;
        group.mn_AndOper=mp_setting->value(sGroupKey,0).toInt(&bOk);
        if( !bOk ) group.mn_AndOper=0;

        sGroupKey=sKey+"group%1";
        sGroupKey=sGroupKey.arg(i);
        int n_count = mp_setting->value(sGroupKey,0).toInt(&bOk);
        if( !bOk ) n_count=0;

        for(int m=0; m<n_count; m++)
        {
            AlarmCondition condition;

            sGroupKey=sKey+"group%1/atype%2";
            sGroupKey=sGroupKey.arg(i).arg(m);

            condition.ms_atype = mp_setting->value(sGroupKey,"").toString();
            //qDebug()<<sGroupKey<<"="<<condition.ms_atype;

            sGroupKey=sKey+"group%1/btype%2";
            sGroupKey=sGroupKey.arg(i).arg(m);
            //qDebug()<<sGroupKey;
            QVariant van = mp_setting->value(sGroupKey,"");
           // qDebug()<<van;
            condition.ms_btype = van.toString();
            //qDebug()<<condition.ms_btype;

            sGroupKey=sKey+"group%1/oper%2";
            sGroupKey=sGroupKey.arg(i).arg(m);
            //qDebug()<<sGroupKey;
            condition.mn_oper = mp_setting->value(sGroupKey,0).toInt(&bOk);
            if( !bOk ) condition.mn_oper=0;

            sGroupKey=sKey+"group%1/compare%2";
            sGroupKey=sGroupKey.arg(i).arg(m);
            //qDebug()<<sGroupKey;
            condition.mn_compare=mp_setting->value(sGroupKey,0).toInt(&bOk);
            if( !bOk ) condition.mn_compare=0;

            sGroupKey=sKey+"group%1/value%2";
            sGroupKey=sGroupKey.arg(i).arg(m);
            //qDebug()<<sGroupKey;
            QString s_value = mp_setting->value(sGroupKey,"0.0").toString();
            condition.mf_value = s_value.toDouble(&bOk);
            if( !bOk ) condition.mf_value=0;

            group.mv_conds.push_back(condition);
        }

        vgroups.push_back(group);
    }
    return 0;*/
}


void CAlarmCfg::check_alarming(QString& s_sn,int n_type,QMap<QString,double> & mapType2Value)
{
 /*   vector<AlarmGroup> vgroups;
    get_group(s_sn,n_type,vgroups);
    QMap<QString,QString>  mapName; //记录报警的元素
    //遍历报警条件  替换标签的为温度值 计算表达式结果
    int n_alarm=0;
    vector<AlarmGroup>::iterator iter = vgroups.begin();
    while( iter != vgroups.end() ) {
        QMap<QString,QString>  mapTemp; //临时记录报警的元素

        AlarmGroup group = (*iter);

        int n_group_alarm=0;
        vector<AlarmCondition>::iterator iter_cond = group.mv_conds.begin();
        while( iter_cond != group.mv_conds.end() ) {
            double d_atype=0;
            double d_btype=0;
            AlarmCondition cond = (*iter_cond);

            QMap<QString,double>::iterator iter_value = mapType2Value.find(cond.ms_atype);
            if( iter_value == mapType2Value.end() ) { iter_cond++;      continue; }

            d_atype = iter_value.value();

            iter_value = mapType2Value.find(cond.ms_btype);

            if( iter_value != mapType2Value.end() ) { d_btype =  iter_value.value(); }
            else if( cond.ms_btype != "" ) { iter_cond++;      continue; };

            double value = d_atype+d_btype*cond.mn_oper;
            if( cond.mn_oper == -1) value=fabs(value);

            int  n_index = cond.mn_compare;

            if( n_index<0 ) n_index=0;
            if( n_index>4 ) n_index=4;

            bool bResult[5]={false,false,false,false,false};

            bResult[0]= value>cond.mf_value ;
            bResult[1]= value<cond.mf_value ;
            bResult[2]= value==cond.mf_value ;
            bResult[3]= value>=cond.mf_value ;
            bResult[4]= value<=cond.mf_value ;


            if( bResult[n_index] ) {
               n_group_alarm++;
               mapTemp[cond.ms_atype]="";
               if(cond.mn_oper !=0 && cond.ms_btype!="" ) {
                   mapTemp[cond.ms_btype]="";
               }
            }

            iter_cond++;
        }//end group

        if(  (group.mn_AndOper!= 0 && n_group_alarm == group.mv_conds.size())
             ||( group.mn_AndOper==0 && n_group_alarm > 0 )  ) {
             n_alarm++;
        } else {
            mapTemp.clear();
        }

        QMap<QString,QString>::iterator iter_name =  mapTemp.begin();
        while( iter_name != mapTemp.end() ) {
            mapName[iter_name.key()]= iter_name.value();
            iter_name++;
        }

        iter++;
    }//end all

    QString str_sn = s_sn+"_real";

    if( n_alarm <=0 || mapName.size() <=0 ) {
        CDataMode::Instance()->reset_all_color(str_sn);
        return ; // no alarm
    }

    if( 1 == get_beep(s_sn,n_type) ) {

        //qDebug() << "alarm check set beep true";
        CDataMode::Instance()->set_beep(true);
    }

    int n_flag = get_capture(s_sn,n_type);
    if( 1 == n_flag  ) {
        QString s_path;
        get_capture_path(s_sn,n_type,s_path);
        if( s_path != "") {
             CDataMode::Instance()->set_capture(str_sn,s_path);
        }
    }

    //name to ana info for splash
    CDataMode::Instance()->set_alalrm_color(str_sn,mapName);*/
}


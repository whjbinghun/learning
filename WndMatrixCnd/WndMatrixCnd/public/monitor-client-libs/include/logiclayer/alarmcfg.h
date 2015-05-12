#ifndef CALARMCFG_H
#define CALARMCFG_H

#include <QtCore>
#include <QSettings>
#include <vector>
#include <QReadWriteLock>

using namespace std;

typedef struct _tagAlmCondition
{
  QString   ms_atype;   //line_no_[max avg min] point_no circle_no rect_no ...
  QString   ms_btype;   //line_no[max avg min] point_no circle_no rect_no ...
  int       mn_oper;    //0 no use; 1 +; -1 -;
  int       mn_compare; //0:>; 1:< ; 2:= ; 3 >=; 4<=
  float     mf_value;
} AlarmCondition;

typedef struct _tagAlmGroup
{
   int                      mn_AndOper;
   vector<AlarmCondition>   mv_conds;
}AlarmGroup;


class CAlarmCfg
{
public:
    static CAlarmCfg* Instance();
    static void Uninstance();

    int  get_capture_path(QString& sSN,int nType,QString& str_path);
    int  set_capture_path(QString& sSN,int nType,QString& str_path);

    int  get_capture(QString& sSN,int nType);
    int  set_capture(QString& sSN,int nType,int nFlag);

    int  get_beep(QString& sSN,int nType);
    int  set_beep(QString& sSN,int nType,int nFlag);

    int  get_message(QString& sSN,int nType,int& nFlag,QString& sMsg);
    int  set_message(QString& sSN,int nType,int nFlag,QString& sMsg);

    int  set_group(QString& sSN,int nType, vector<AlarmGroup>& vgroups);
    int  get_group(QString& sSN,int nType, vector<AlarmGroup>& vgroups);

    void check_alarming(QString& s_sn,int n_type,QMap<QString,double> & mapType2Value);
private:
   CAlarmCfg();
   virtual ~CAlarmCfg();
   QReadWriteLock    m_rwLock;
   static CAlarmCfg* m_pInstance;

   QSettings    *mp_setting;
};

#endif // CALARMCFG_H

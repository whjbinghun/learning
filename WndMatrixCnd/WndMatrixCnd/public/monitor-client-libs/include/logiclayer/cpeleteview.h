#ifndef CPELETEVIEW_H
#define CPELETEVIEW_H
#include "cloginview.h"
#include "datamode.h"

class CPeleteView : public CLoginView
{
public:
    CPeleteView();
    virtual ~CPeleteView();
    
    int  get_pelete_clr(QString& s_sn);
    int  set_pelete_auto(QString& s_sn);
    int  set_pelete_clr(QString& s_sn,int n_clr);
    int  set_pelete_range(QString& s_sn,float & fMin,float & fMax);

    virtual int onResponse(CBaseMsg& reqMsg,CBaseMsg& rspMsg);
    virtual int onNotify(CBaseMsg& msg);

};

#endif // CPELETEVIEW_H

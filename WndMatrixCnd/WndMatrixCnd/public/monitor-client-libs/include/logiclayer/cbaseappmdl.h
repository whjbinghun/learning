#ifndef CBASEAPPMDL_H
#define CBASEAPPMDL_H
#include "cbasemdl.h"
#include "cbaseview.h"
#include <map>
using namespace std;
class CMsLogiclayer;
typedef map<int,CBaseView*> MapView;

class CBaseAppMdl : public CBaseMdl
{
public:
    CBaseAppMdl();
    virtual ~CBaseAppMdl();

    static CBaseAppMdl* Instance();
    static void Uninstance();

    virtual int postMsg(CBaseMsg& msg);
    virtual int start(int n_stacksize,int n_priority);
    virtual int stop();
    int     addView(CBaseView* pView);
    void    removeView(int nId);

//Q_SIGNALS:
//      void  sig_onMsgIn();
//public Q_SLOTS:
//        void slot_onMsgIn();
protected:
    virtual int handleNotify(CBaseMsg &msg);
    virtual int handleResponse(CBaseMsg &msgRsp);
protected:
    static CBaseAppMdl* m_pInstance;
    int  m_nViewIdGenerator;
    CMsLogiclayer* m_pLogic;

    MapView        m_mapView;
};

#endif // CBASEAPPMDL_H

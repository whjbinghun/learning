#ifndef CBASEVIEW_H
#define CBASEVIEW_H
#include "cbasemsg.h"

class CBaseView
{
public:
    CBaseView();
    virtual ~CBaseView();

    virtual int onResponse(CBaseMsg& reqMsg,CBaseMsg& rspMsg);
    virtual int onNotify(CBaseMsg& msg);

protected:
   int m_nViewId;
};

#endif // CBASEVIEW_H

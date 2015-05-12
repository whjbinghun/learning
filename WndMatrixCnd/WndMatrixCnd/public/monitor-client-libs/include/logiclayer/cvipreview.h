#ifndef CVIPREVIEW_H
#define CVIPREVIEW_H
#include "cloginview.h"

class CViPreview : public CLoginView
{
public:
    CViPreview();
    virtual ~CViPreview();

    virtual int onResponse(CBaseMsg& reqMsg,CBaseMsg& rspMsg);
    virtual int onNotify(CBaseMsg& msg);

    int focus_in( QString &str_sn );
    int focus_out( QString &str_sn );
    int focus_in_stop( QString &str_sn );
    int focus_out_stop( QString &str_sn );
};

#endif // CVIPREVIEW_H

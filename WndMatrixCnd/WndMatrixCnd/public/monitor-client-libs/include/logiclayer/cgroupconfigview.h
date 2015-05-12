#ifndef CGROUPCONFIGVIEW_H
#define CGROUPCONFIGVIEW_H
#include "cloginview.h"

class CGroupConfigView : public CLoginView
{
public:
    CGroupConfigView();
    virtual ~CGroupConfigView();

    int add_group( int n_count, QString &str_sns, QString &str_name );
    int change_group( int n_group, int n_count, QString &str_sns, QString &str_name );
    int del_group( int n_group );

    virtual int onResponse(CBaseMsg& reqMsg,CBaseMsg& rspMsg);
    virtual int onNotify(CBaseMsg& msg);

};

#endif // CGROUPCONFIGVIEW_H

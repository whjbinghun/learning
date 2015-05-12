#ifndef CIRPREVIEW_H
#define CIRPREVIEW_H
#include "cloginview.h"
#include "datamode.h"
#include "cptzrailview.h"

class CIrPreview : public CPtzRailView
{
public:
    CIrPreview();
    virtual ~CIrPreview();

    int load_ana( int n_type, int n_group_num, QString &str_sn, QList< AnaInfo > &list_anas );\

    int add_ana( int n_type /*1 live ; 2 replay */, int n_group_num, QString &str_sn, QList< AnaInfo > &list_ana, bool b_change_ana = false );
    int del_ana( int n_type, int n_group_num, QString &str_sn, QList< AnaInfo > &list_ana, bool b_change_ana = false );
    int change_ana( int n_type, int n_group_num, QString& s_sn, AnaInfo& old_ana, AnaInfo& new_ana );

    int focus_in( QString &str_sn );
    int focus_out( QString &str_sn );
    int correct_ir( QString &str_sn );

    virtual int onResponse(CBaseMsg& reqMsg,CBaseMsg& rspMsg);
    virtual int onNotify(CBaseMsg& msg);

};

#endif // CIRPREVIEW_H

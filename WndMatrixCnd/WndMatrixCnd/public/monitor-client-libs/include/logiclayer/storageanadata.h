#ifndef STORAGEANADATA_H
#define STORAGEANADATA_H

#include "datacommon.h"

//typedef struct
//{

//};

class StorageAnaDataBase
{
public:
    virtual void load( const int n_ana_id, QList< YF_ANA_CHART_TREND_POINT > & list_pts ) = 0;
    virtual void save( const int n_ana_id, const YF_ANA_CHART_TREND_POINT pts ) = 0;
};

class FileStorageAnaData: public StorageAnaDataBase
{
public:
    void load( const int n_ana_id, QList< YF_ANA_CHART_TREND_POINT > & list_pts );
    void save( const int n_ana_id, const YF_ANA_CHART_TREND_POINT pts );
};

class DatabaseStorageAnaData: public StorageAnaDataBase
{
public:
    void load( const int n_ana_id, QList< YF_ANA_CHART_TREND_POINT > & list_pts );
    void save( const int n_ana_id, const YF_ANA_CHART_TREND_POINT pts );
};

#endif // STORAGEANADATA_H

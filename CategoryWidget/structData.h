#ifndef STRUCTDATA_H
#define STRUCTDATA_H

#include <QVariant>
#include <QDateTime>
#include <QVector>
#include "enumData.h"

struct GoodsInfo
{
    int nId;//商品id
    QString strCode;//商品编码id
    QString sBarCode;//商品条码
    QString sName;//商品名称
    QString sShortName;//商品简称
    int nCategoryId;//商品分类id
    float fPrice;//商品价格
    float fMemberPrice; // 会员价
    int nTasteType;//商品口味分级
    bool bStandard;//商品类型(标品，非标品）
    QString sSaleUnit;//销售单位： 元/公斤
    bool bPromotion; // 是否促销
    bool bAllowSell;//是否允许售卖

    GoodsInfo()
        : nId( -1 )
        , strCode( "" )
        , sBarCode( "" )
        , sName( "" )
        , sShortName( "" )
        , nCategoryId( -1 )
        , fPrice( 0.00 )
        , fMemberPrice( 0.00 )
        , nTasteType( 0 )
        , bStandard( false )
        , sSaleUnit( "公斤" )
        , bPromotion( false )
        , bAllowSell( true ) {}
};

struct CategoryInfo
{
    int nId; //商品分类id   nCategoryId
    QString sName; //商品分类名
    bool bCustom;//是否为自定义
    QString sKeyId; // 唯一标识
    bool bSelected; //选中状态
    QList<GoodsInfo> listGoodsInfo;

    CategoryInfo()
        : bSelected( false ) {}
};

//加上下面的语句，CategoryInfo结构体可以自动转换成QVariant结构；否则不能自动转换，并会报错
Q_DECLARE_METATYPE(CategoryInfo)

#endif

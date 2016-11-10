#ifndef ENUMDATA_H
#define ENUMDATA_H

// 数据库类型
enum DatabaseType{
    e_sqlite,
    e_sqlServer
};

enum ServerHttpErrorCode{
    e_connectServerError = -1,
    e_success = 0, // 成功
    e_paramError = 1, // 参数错误，未找到参数
    e_paramSignError = 2, // 数字签名错误
    e_accessTokenError = 3,// access_token,错误
    e_noAccessPos = 4, // POS授权错误
    e_userNameOrPasswordError = 1001, // 用户名或密码错误
    e_userPosNotBelongShopError = 1002, // 用户和Pos不属于同一门店
    e_corpIDError = 3001, // 企业编号错误
    e_shopIDError = 3002, // 门店编号错误
    e_posRegisted = 4001, // 设备已注册
    e_posNotRegist = 5001, // 设备未注册
    //01001.
};

enum PosRegistState{
    e_checked = 1, // 审核通过
    e_checking = 2, // 正在审核
};

//商品分类窗口
enum CategoryWidgetType {
    e_categoryNull = 0,
    e_categorySale = 1,
    e_categorySet = 2,
};

//商品窗口
enum GoodsWidgetType {
    e_goodsSale = 10,
    e_goodsSet = 11,
};

//商品类型
enum GoodsType {
    e_goodsStandard = 1,//标品
    e_goodsNoStandard = 2, //非标品
};

enum ESaleHeadStatus {
    e_headCancel = -1, //取消
    e_headComplete = 1, //完成
    e_headSelling = 2,//正在进行
    e_headPending = 3, //挂单
};

//性别
enum ESex
{
    e_nullSex =0, // 保密
    e_man, // 男
    e_woman, // 女

};

//更新类型
enum KPosUpdateType
{
    e_noUpdate, // 不更新
    e_install, // 安装包更新
    e_increment, // 增量更新
};

//支付方式
enum EPayMethod {
    PayNull = 0,
    PayCash = 1, //现金支付
    PayMemCard = 2, //会员卡支付
    PayStoreCard = 3, //储值卡支付,银联
    PayAlipay = 4, //支付宝
    PayWeChat = 5, //微信支付
    ApplePay = 6, //苹果支付
    JdWallet = 7, //京东钱包

    PayFrontPage = 100,
    PayNextPage = 101,
};

//支付类型
enum EPaymentType {
    e_Payment=1, //付款
    e_Refund=2, //退款
};

// 支付状态
enum EPaymentState{
    e_PayFailed =0 ,
    e_PaySuccess = 1,
    e_PayWaiting = 2,//正在进行中
    e_PayRefund = 3,//已退款
};

//结算状态
enum EStatementStatus {
    StatementFailed = 0, //结算失败
    StatementSuc = 1, //结算成功
    StatementWaiting = 2, //等待中
    StatementScaning = 3  //请扫码
};

//退款状态
enum ERefundStatus {
    RefundFailed = 0, //退款失败
    RefundSuc = 1, //退款成功
    RefundWaiting = 2 //退款中
};

//支付状态
enum ETradeStatus {
    eTradeSuc = 0, //支付成功
    eTradeWaiting = 1, //用户支付中
    eTradeFailed = 2, //支付失败
    eTradeRevoke = 3, //支付已撤销
    eTradeTransferRefund = 4, //转入退款
    eTradeNoPay = 5, //未支付
    eTradeClosed = 6,   //已关闭
    eTradeRefund = 7  //7.未付款交易超时关闭，或支付完成后全额退款
};

//小票上传状态
enum EUploadReceiptStatus {
    eNotUpload = 0, //未上传
    eUploaded = 1, //已上传
    eUploading = 2 //正在上传中
};

enum EButtonStatus {
    eDisAble = 0, //无效
    ePress = 1, //选中
    eNormal = 2, //未选中
};

// 皮肤类型
enum KPosSkinType
{
    e_blackSkin = 1, // 黑色版
    e_flatSkin, // 扁平版
    e_smartSkin, // 智能版
};

// 首页按键类型
enum HomeKeyType
{
    e_homeElse = 1, // 其他
    e_homeMember = 2, // 会员
    e_homePending = 3, // 挂单
    e_homeCancel = 4, // 取消
    e_homeTrimMoney = 5, // 整取
    e_homeDiscount = 6, // 折扣
    e_lastReceipt = 7, // 上张小票
    e_homeFastCreateDocument = 8, // 快速建档
    e_homeTrimBox = 9, // 去皮
    e_homeDiscountAll = 10, // 整单折扣
    e_homePurse = 11, // 钱箱
    e_homeRefundGoods = 12, // 退货
    e_homeLock = 13, // 锁屏
    e_homeModifyNum = 14 ,//修改商品数量
};

// 退款按键类型
enum RefundKeyType
{
    e_refundElse = 15, // 其他
    e_refundGoods, // 退货
    e_refundAllGoods, // 整单退货
    e_refundMember, // 会员
    e_refundReason1, // 原因一
    e_refundReason2 = 20, // 原因二
    e_refundReason3, // 原因三
    e_refundReason4, // 原因四
};

// 系统参数
//enum SystemParamsType
//{
//    e_bDiscountOnMember, // 是否在会员价基础上打折
//    e_bOverOriginPriceAfterDiscount, // 折后金额是否能高于原价
//    e_bOpenOfflineMember, // 是否开启离线会员
//    e_nGoodsNumberDecimalsLength, // 商品数量的小数位数
//    e_nGoodsPriceDecemalsLength, // 商品金额的小数位数
//    e_nLocalSaleDataSaveDay, // 本地销售数据保存时间
//    e_bRandomSaleOrderCode, // 随机销售订单编码
//    e_nHintModifyPasswordDay, // 提示修改密码的间隔天数
//    e_nSingleGoodsMaxNum, // 销售时单品最大数量
//    e_bAllowSellZeroPriceGoods, // 是否允许销售0元商品
//    e_bChoosePayWayWhenRefund, // 是否退货时选择付款方式
//    e_bAllowEditCardNumber, // 是否启动键盘输入会员号/储值卡
//    e_nSingleGoodsDiscountBase, // 单品打折基础
//    e_bAllowFastCreateDocument, // 是否允许快速建档自建档案
//    e_bAllowFastChangePrice, // 是否允许快速改价
//    e_bTempChagePriceWhenFast, // 快速改价是否临时改价
//    e_nMemberCardNumberLength, // 会员找好长度
//    e_bMustWriteRefundReason, // 退货原因是否必选
//};

// 门店参数
enum ShopParamsType
{
    e_bOpenPurse = 1, // 是否开启零钱包
    e_bOneKeyBuyGoods, // 是否一键购买商品
    e_bAllowGoodsRepeatAdd, // 商品是否叠加
    e_bNotPopHintWhenNoChange, // 找零金额为零不弹提示框
    e_bLockScreen, // 是否锁屏
    e_bHintWhenUnderLowestPrice, // 单笔议价/折扣后单价低于进价提示
    e_bChooseSalesMen, // 是否选择业务员
    e_nPendingListMaxLength, // 最大挂单数
    e_nDefaultPrettyCash, // 默认备用金
    e_nLockWaitTimeLenth, // 锁屏等待时间
};

enum CheckOrderGoodsSortType
{
    e_sortByPrice, // 按金额
    e_sortByNumber, // 按数量
};

// 收货状态
enum RecieveGoodsState
{
    e_RecieveGoodsFinish, // 已完成
    e_RecieveGoodsDoing, // 收货中
    e_RecieveGoodsNotStart, // 未收货
};

//会员
enum MemCardType {
    e_nullMem = 0,//非会员
    e_commonMem = 1,//普通会员
    e_platinumMem = 2,//白金会员
    e_diamondMem = 3, //钻石会员
};

// 收货时商品称重类型
enum ReceiveWeighType
{
    e_receiveWeighNull, // 空值
    e_standardWeigh, // 标品称重
    e_noStandardConfirmWeigh, // 非标品确定量称重
    e_noStandardNoConfirmWeigh, // 非标品不确定量称重
    e_disperseWeigh, // 非标品散重称重
};

// 收货单据类型
enum ReceiveOrderType
{
    e_orderOrder, // 订单
    e_manualOrder, // 手工
    e_backDelivery, // 返配
    e_allocateTransfer, // 调拨
};

// 盘点类型
enum StockingOrderType
{
   e_systemStocking,  // 总部盘点
   e_tempStocking, // 临时盘点
   e_cycleStocking, // 循环盘点
};

// 盘点状态
enum StockingOrderState
{
    e_stockingNoStart, // 未开始
    e_stockingDoing, // 进行中
    e_stockingHaveDiffer, // 出差异
    e_stockingAdjust, // 已调整
    e_stockingDrop, // 废弃
};

//证件类型
enum EDocumentType {
    eIdentityCard = 0,//身份证
    eOfficerCard = 1,//军官证
    ePassportCard = 2 ,//护照
    eStudentCard = 3 ,//学生证
    eOtherCard = 4 //其他
};

//卡状态
enum ECardStatus {
    eRefundedCard = -1,//退卡
    eReportLossCard = 0,//挂失
    eNormalCard = 1,//启用
};

#endif

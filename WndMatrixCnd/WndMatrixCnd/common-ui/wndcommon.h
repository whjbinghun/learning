#ifndef WNDCOMMON_H
#define WNDCOMMON_H

#define YF_DATA_GROUP_NUM 50    //组列表中，item的DATA值

typedef struct _tagPicPreviewInfo{
    QImage image;
    QWidget *p_widget;
    QString str_path;
    QString str_filename;//
    QString str_sn;
    int     n_class; //2 : ptz 3: rail_v 4: rail_h
    bool b_have_ptz;//判断有无云台
    _tagPicPreviewInfo()
        //: p_image(0)
        : p_widget( 0 )
        , str_path("")
        , str_filename("")
        , str_sn("")
        , n_class(-1)
        , b_have_ptz( false )
        {}
} PicPreviewInfo;

typedef struct _tagPicPreviewGroupInfo {
    PicPreviewInfo pic_vi;//可见光
    PicPreviewInfo pic_ir;//红外
    PicPreviewInfo pic_cloud_pan;//云台
    PicPreviewInfo pic_guid_rail;//导轨
    QString str_group_name;//组别名
    //QString str_name;//如果没有组别名或者组号<0，那么使用str_name保存
    int n_group_num;

    _tagPicPreviewGroupInfo()
        : str_group_name("")
        , n_group_num(0){ }
} PicPreviewGroupInfo;

enum DeviceClass{
    IR = 0L,
    VI = 1L,
    PTZ = 2L,
    RAIL_H=3L,
    RAIL_V=4L
};

enum AnaMove{
    move_null = 0L,
    move_self,
    move_left,
    move_lefttop,
    move_top,
    move_righttop,
    move_right,
    move_rightbottom,
    move_bottom,
    move_leftbottom
};

/*typedef struct {
    AnaInfo ana_info;
    bool b_add_or_del;
    int n_group_num;
    int n_index;
} AnaCtrlAddOrDel;

typedef struct {
    AnaInfo ana_info_old;
    AnaInfo ana_info_new;
    int n_group_num;
    int n_index;
} AnaCtrlChange;
*/

typedef struct _tagRailCtrlInfo{
    int n_msg_type;
    int n_result, n_group_num;
    unsigned int n_info, n_info_oth;

    _tagRailCtrlInfo()
        : n_msg_type(-1)
        , n_group_num(0)
        , n_result(-1)
        , n_info(0)
        , n_info_oth(0){}
} RailCtrlInfo;


//typedef struct _tagDeviceInfo{
//    int n_class;
//    int n_port;
//    QString str_ip;
//    QString str_sn;
//    QString str_type;
//    QString str_mac;
//    _tagDeviceInfo()
//        : n_class(0), n_port(0)
//        , str_ip(""), str_sn("")
//        , str_type(""), str_mac(""){}
//}DeviceInfo, *PDeviceInfo;

//typedef struct _tagDeviceGroup{
//    DeviceInfo dev_ir;
//    DeviceInfo dev_vi;
//    DeviceInfo dev_ptz;
//    DeviceInfo dev_rail;
//} DeviceGroup;

enum AnaType{
    ANA_NULL = 0L,
    ANA_COM = 1L,//普通分析
    ANA_MATRIX = 2L,//阵列分析
    ANA_INTERIM = 3L,//临时分析
};


#endif // WNDCOMMON_H

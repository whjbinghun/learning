#ifndef WNDCOMMON_H
#define WNDCOMMON_H

typedef struct _tagPicPreviewInfo{
    QImage *p_image;
    QString str_path;
    QString str_filename;//
    QString str_sn;
    _tagPicPreviewInfo()
        : p_image(0)
        , str_path("")
        , str_filename("")
        , str_sn(""){}
} PicPreviewInfo;

typedef struct _tagPicPreviewGroupInfo {
    PicPreviewInfo pic_vi;//可见光
    PicPreviewInfo pic_ir;//红外
    PicPreviewInfo pic_cloud_pan;//云台
    QString str_group_name;
    int n_group_num;

    _tagPicPreviewGroupInfo()
        : str_group_name("")
        , n_group_num(0){ }
} PicPreviewGroupInfo;

enum DeviceClass{
    IR = 0L,
    VI = 1L,
    PTZ = 2L
};

#endif // WNDCOMMON_H

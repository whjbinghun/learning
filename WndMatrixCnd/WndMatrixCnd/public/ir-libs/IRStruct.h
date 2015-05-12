
#ifndef _IR_STRUCT_H_
#define _IR_STRUCT_H_

#define ERRORTEMP	-99999.0f

#ifndef _YLINETBL_DEF_
#define _YLINETBL_DEF_

typedef struct _LineTbl{
    unsigned short	w_height;
    unsigned short	w_start,w_end;
    _LineTbl() : w_height( 0 ), w_start( 0 ), w_end( 0 ){
    }
}YLineTbl,*PYLineTbl;

#endif

#ifndef _YFTEMPERATURE_DEF_
#define _YFTEMPERATURE_DEF_

typedef struct tagYFTemperature{
    float	ft_temp;
    unsigned short	i_x, i_y;
    unsigned short	w_ad;
    tagYFTemperature() : ft_temp( -99999.0f ), i_x( 0 ), i_y( 0 ), w_ad( 0 ){
    }
}YFTemperature,*PYFTemperature;

#endif

#ifndef _YFIMAGING_STRUCT_
#define _YFIMAGING_STRUCT_

typedef struct tagAD2Pixel{
    unsigned short *pw_ad;
    unsigned short	w_img_width,w_img_height;
    unsigned char *pby_bits;
    int	i_bit_count;
    bool b_3d;
    tagAD2Pixel() : pw_ad( 0 ), pby_bits( 0 ), b_3d( false ),
        w_img_width( 0 ), w_img_height( 0 ), i_bit_count( 0 ){
    }
    bool is_valid(){
        return ( pw_ad && pby_bits && w_img_width > 0 && w_img_height > 0 );
    }
}YFAD2Pixel,*PYFAD2Pixel;
#endif
/**
    @enum 颜色的索引
*/
#ifndef _YF_CLR_ADJUST_STRUCT_
#define _YF_CLR_ADJUST_STRUCT_
#define COLOR_DEPTH 256
enum ColorIndex
{
    clrNone=-1,
    clrIron=1,					///< 铁红
    clrRainbow=2,				///< 彩虹
    clrFeather=3,				///< 医疗
    clrGray=4,					///< 灰度
    clrIronReverse=5,		///< 反转铁红
    clrRainbowReverse=6,///< 反转彩虹
    clrFeatherReverse=7,	///< 反转医疗
    clrGrayReverse=8,		///< 反转灰度
};

enum YFAdjustType{
    AdjustAuto=0,			///< 自动调节
    AdjustManual=1,		///< 手动调节
    AdjustUseFile=2,	///< 按照文件录制时调节
};

#endif

enum YFRecordType{
    RecordIRV = 0,
    RecordAVI = 1,
    RecordIRVAndAVI = 2,
};

#ifndef _ENUM_YF_LEN_TYPE_
#define _ENUM_YF_LEN_TYPE_
enum YFLenType{
    Len24 = 0,
    Len12 = 1,
    Len7 = 2,
    Len48 = 3,
    LenMacro100 = 4, ///< 100微米微距镜头
};
#endif

#ifndef _ENUM_YF_MEAS_GRADE_
#define _ENUM_YF_MEAS_GRADE_
#define TOTAL_MEAS_GRADE 3
enum YFMeasGrade{
    MeasGradeLow = 0,
    MeasGradeMid = 1,
    MeasGradeHigh = 2,
};
#endif

#ifndef YFSHOWTEXTTYPE
#define YFSHOWTEXTTYPE
enum ShowTextType
    {
        ShowNone,
        ShowMaxTemp,
        ShowMinTemp,
        ShowAvgTemp,
        ShowAll
    };
#endif

#ifndef _ENUM_YF_ROTATE_ANGLE_
#define _ENUM_YF_ROTATE_ANGLE_
enum RotateAngle{
    Right90 = 0,	///< 右转90度
    Left90 = 1,		///< 左转90度
    Right180 = 2,	///< 即180度
};
#endif

/**
    @brief 有关分析的操作，包括增加、删除和更新 Add by ideal 2012-10-17
    WM_YF_ANA 代表点、线、框、圆、多边形和折线等
    WM_YF_DW 代表等温分析
    WM_YF_ANA_DEL_ALL 代表

    发送消息的格式如下：消息类型 分析类的指针 针对分析的操作
    PostMessage(hWnd,WM_YF_ANA,YFIRPoint*,YFAnaAdd);
*/
#define WM_USER					0x0400
#define WM_YF_ANA				WM_USER+8016	///< 发送消息时，在WPARAM中
#define WM_YF_DW				WM_USER+8017	///<
#define WM_YF_ANA_DEL_ALL		WM_USER+8018	///<
enum YFAnaOperation{
    YFAnaNULL = -1,
    YFAnaAdd = 0,
    YFAnaDelete = 1,
    YFAnaUpdate = 2,
    YFAnaUpdateSize = 3,
    YFAnaAddCursor = 4,
    YFAnaDeleteCursor = 5,

    YFAnaAddDW = 6,
    YFAnaDeleteDW = 7,
    YFAnaUpdateDW = 8,
    YFAnaDeleteAllDW = 9,

    YFAnaSelected = 10,
};

///< Add by idal 20121228 用于将分析的主要信息通过消息发送给通知窗口
typedef struct tagYFAnaMsg
{
    int		ana_type;	///< 分析的类型
    void*	h_handle;
}YFAnaMsg,*PKCAnaMsg;
#endif

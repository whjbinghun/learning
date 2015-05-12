
#ifndef _YF_IR_IMAGE_EX1_HEADER_
#define _YF_IR_IMAGE_EX1_HEADER_

#include <string>

#ifdef WIN32
#ifdef	YFIRIMAGEEX1_EXPORTS
#define YFDLL_YFIRIMAGEEX1_API __declspec(dllexport) bool __stdcall
#else
#define YFDLL_YFIRIMAGEEX1_API __declspec(dllimport) bool __stdcall
#endif
#else
#define YFDLL_YFIRIMAGEEX1_API bool
#endif

#ifndef DEFINE_YHANDLE
typedef void *YHANDLE;
#define DEFINE_YHANDLE
#endif

#ifndef _YFIMAGING_STRUCT_
#define _YFIMAGING_STRUCT_

typedef struct tagDW{
    unsigned short w_down_ad, w_up_ad;
    unsigned int dw_clr;
    bool b_trans;
}YDW, *PYDW;

/**
    @enum 颜色的索引
*/
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

#ifndef _ENUM_YF_ANA_TYPE_
#define _ENUM_YF_ANA_TYPE_
enum YFAnaType{	///< 分析的类型
    YFAnaTypeDelete=0,	///< 删除分析

    YFPoint=10,	///< 选中点
    YFPointMax=11,	///< 选中最高温度点
    YFPointMin=12,	///< 选中最低温度点
    YFPointMove=13,	///< 移动点
    YFPointDelete=14,	///< 删除点
    YFPointDeleteMax=15,	///< 删除最高温度点
    YFPointDeleteMin=16,	///< 删除最低温度点

    YFLine=20,	///< 选中直线
    YFLineStart=21,	///< 选中直线的起始点
    YFLineEnd=22,	///< 选中直线的结束点
    YFLineMove=23,	///< 移动直线
    YFLineDelete=24,	///< 删除直线
    YFLineCursor=25,	///< 选中直线上的游标
    YFLineCursorDel=26,	///< 删除直线上的游标
    YFLineCursorMoveX=27,	///<
    YFLineCursorMoveY=28,

    YFEllipse=30,	///< 选中圆，不用Circle是考虑到以后可能会使用椭圆
    YFEllipseBorderNW=31,	///< 选中圆的西南（东北也一样）
    YFEllipseBorderNE=32,	///< 选中圆的东南（西北也一样）
    YFEllipseMove=33,	///< 移动圆
    YFEllipseDelete=34,	///< 删除圆

    YFRect=40,	///< 选中框
    YFRectLT=41,	///< 选中框的左上角
    YFRectT=42,	///< 选中框的上边
    YFRectRT=43,	///< 选中框的右上角
    YFRectR=44,	///< 选中框的右边
    YFRectRB=45,	///< 选中框的右下角
    YFRectB=46,	///< 选中框的下边
    YFRectLB=47,	///< 选中框的左下角
    YFRectL=48,	///< 选中框的左边
    YFRectMove=49,	///< 移动框
    YFRectDelete=400,	///< 删除框

    YFPoly=50,	///< 选中多边形
    YFPolyBorder=51,	///< 选中多边形的一条边
    YFPolyPoint=52,	///< 选中多边形的一个点
    YFPolyMove=53,	///< 移动多边形
    YFPolyDelete=54,	///< 删除多边形

    YFPolyline=60,	///< 选中折线
    YFPolylineBorder=61,	///< 选中折线的一条边
    YFPolylinePoint=62,	///< 选中折线的一个店
    YFPolylineMove=63,	///< 移动折线
    YFPolylineDelete=64,	///< 删除折线

    YFWholeImage=100	///< 不在分析工具上
};
#endif

#ifndef _IR_STRUCT_H_
#define _IR_STRUCT_H_

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

#endif

/**
	@brief 封装与美盛红外图片相关的接口，实际使用可参考TestIRImageEx1例程 by ideal 2013-12-05

	***以下所有接口的返回值，成功调用返回true，失败返回false
	***参数h，标识红外图片的句柄
*/

/**
	@brief 初始化和卸载

    yf_ir_image_ex1_is_yf_file：查看是否是美盛公司的红外文件

	@param h 成功返回图像类的句柄，否则返回0
*/
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_init( YHANDLE &h );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_uninit( YHANDLE &h );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_is_yf_file( const std::string &str_file_name );

/**
	@brief 加载和存储红外图片

    yf_ir_image_ex1_load_file：加载红外图片
    yf_ir_image_ex1_save_file：存储irp格式的红外图片
    yf_ir_image_ex1_append_ad_to_file：附加温度信息到BMP或JPG文件中

	@param str_file_name 符合美盛公司红外图片格式的文件
*/
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_load_file( YHANDLE h, const std::string &str_file_name );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_save_file( YHANDLE h, const std::string &str_file_name );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_append_ad_to_file( YHANDLE h, const std::string &str_file_name );

/**
	@brief 获取当前图片的分辨率

	@param w_width，w_height 返回宽度和高度
*/
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_image_resolution( YHANDLE h, unsigned short &w_width, unsigned short &w_height );
/**
	@brief 获取当前图片的机器序列号

	@param str_serial 返回机器序列号，格式是“生产厂商+”-“+机器类型+”-“+机器编号
*/
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_mac_serial( YHANDLE h, std::string &str_serial );
/**
	@brief 获取当前红外图片的像素数据

	@param pby_bits 由应用程序分配的缓冲区，用来存储像素数据
	@param dw_size 无论pby_bits是否为空，dw_size均返回像素数据的字节大小

	***目前仅支持32位像素，即每个像素点占4个字节
	***pby_bits为空时，可通过该函数获取当前图像像素数据所占的字节大小
*/
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_bits( YHANDLE h, unsigned char *pby_bits, unsigned int &dw_size );

/**
	@brief 以下10个接口用来调节红外图像的显示

	@param index ColorIndex枚举类型
	@param i_min，i_max 返回图像调节的数据值
	@param ft_min，ft_max 返回图像调节的温度值
	@param type YFAdjustType枚举类型

	s/get_color_index 获取或改变图像的伪彩显示
	s/get_adjust_param 获取或改变当前图像调节所采用的数据范围
	s/get_adjust_temp 获取或改变当前图像调节所采用的温度范围
	s/get_adjust_type 获取或改变当前图像调节的类型
	get_org_adjust_param 获取红外图片中保存图像调节的数据范围
	update_org_adjust_param 更新红外图片中保存的图像调节的数据范围，需要调用save_file后才真正更新到文件中
*/
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_color_index( YHANDLE h, ColorIndex &index );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_adjust_param( YHANDLE h, int &i_min, int &i_max );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_adjust_temp( YHANDLE h, float &ft_min, float &ft_max );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_org_adjust_param( YHANDLE h, int &i_min, int &i_max );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_adjust_type( YHANDLE h, YFAdjustType &type );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_set_color_index( YHANDLE h, ColorIndex index );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_set_adjust_param( YHANDLE h, int i_min, int i_max );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_set_adjust_temp( YHANDLE h, float ft_min, float ft_max );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_update_org_adjust_param( YHANDLE h, int i_min, int i_max );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_set_adjust_type( YHANDLE h, YFAdjustType type );

/**
	@brief 以下10个接口用来获取或设置红外的测温参数
	
	@param b_fahrenheit 华氏或摄氏
	@param ft_emiss 辐射率，范围0~1
	@param ft_dist 距离，单位米
	@param ft_env_temp 环境温度，单位℃
	@param ft_rel_hum 相对湿度，范围0~1
	@param ft_mdf 修正温度，范围-100~100
*/
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_set_fahrenheit( YHANDLE h, bool b_fahrenheit );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_set_emiss( YHANDLE h, float ft_emiss );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_set_dist( YHANDLE h, float ft_dist );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_set_env_temp( YHANDLE h, float ft_env_temp );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_set_rel_hum( YHANDLE h, float ft_rel_hum );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_set_mdf( YHANDLE h, float ft_mdf );

YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_fahrenheit( YHANDLE h, bool &b_fahrenheit );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_emiss( YHANDLE h, float &ft_emiss );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_dist( YHANDLE h, float &ft_dist );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_env_temp( YHANDLE h, float &ft_env_temp );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_rel_hum( YHANDLE h, float &ft_rel_hum );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_mdf( YHANDLE h, float &ft_mdf );

/**
	@brief 获取整幅图像的最高温度、最低温度和每个点的温度

	@param w_x，w_y 坐标值
	@param ft_temp 返回温度值

	get_max_temp 获取整幅图像的最高温度及坐标
	get_min_temp 获取整幅图像的最低温度及坐标
	get_temp_from_loc 获取图像中具体坐标点的温度值
*/
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_max_temp( YHANDLE h, unsigned short &w_x, unsigned short &w_y, float &ft_temp );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_min_temp( YHANDLE h, unsigned short &w_x, unsigned short &w_y, float &ft_temp );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_temp_from_loc( YHANDLE h, unsigned short w_x, unsigned short w_y, float &ft_temp );

/**
	@brief 以下23个接口负责与分析相关的接口，字符串的格式详见YFFormatAnaStr.h

	@param str_anas 单个或多个分析组成的格式化字符串
	@param str_ana 单个分析的格式化字符串
	@param b_show 获取或设置是否显示
	@param w_x，w_y 返回坐标数据
	@param ft_temp 返回温度值
	@param pft_temp 由应用程序分配的缓冲区，存储分析中每个点的温度值
	@param dw_count 分析中温度点的个数
	@param index ColorIndex枚举类型
	@param i_min，i_max 返回图像调节的数据值
	@param ft_min，ft_max 返回图像调节的温度值
	@param b_has 获取或设置分析具有某些属性
	@param str_alias 获取或设置分析的别名
	@param str_des 获取或设置分析的备注

	load_anas_from_str 加载格式化字符串，加载之前会首先删除点原先所有的分析
	get_anas_str 获取当前图片所有分析组成的格式字符串
	add_ana 通过格式化字符串，增加单个分析
	delete_ana 通过格式化字符串，删除单个分析
	delete_all_anas 删除图片中的所有分析

	show_max/in 设置显示最高/低温度点
	is_show_max/in 获取是否显示最高/低温度点
	get_ana-max/in/avg_temp 获取分析中的最高、最低和平均温度
	get_ana_all_temp 获取分析所有点的温度

	s/get_ana_color_index 获取和设置分析内图像的伪彩
	ana_has_self_color 获取分析是否有私有调色板
	set_ana_has_self_color 设置分析是否有私有调色板
	***若要使分析内的伪彩显示起作用，分析必须有私有调色板，即必须调用set_ana_has_self_color为true

	ana_self_can_adjust 是否可以调节分析的图像
	set_ana_has_self_adjust 设置是否可以调节分析的图像
	s/get_ana_adjust_param 获取或改变当前分析内图像调节所采用的数据范围
	s/get_ana_adjust_temp 获取或改变当前分析内图像调节所采用的温度范围

	s/get_ana_alias 获取或设置分析的别名
	s/get_ana_des 获取或设置分析的备注
*/
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_load_anas_from_str( YHANDLE h, const std::string &str_anas );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_anas_str( YHANDLE h, std::string &str_anas );

YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_add_ana( YHANDLE h, const std::string &str_ana );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_delete_ana( YHANDLE h, const std::string &str_ana );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_delete_all_anas( YHANDLE h );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_hit_type( YHANDLE h, unsigned short w_x, unsigned short w_y, bool b_delete, YFAnaType &type );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_hit_ana_str( YHANDLE h, unsigned w_x, unsigned short w_y, std::string &str_ana );

YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_ana_show_max( YHANDLE h, const std::string &str_ana, bool b_show );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_ana_show_min( YHANDLE h, const std::string &str_ana, bool b_show );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_ana_is_show_max( YHANDLE h, const std::string &str_ana, bool &b_show );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_ana_is_show_min( YHANDLE h, const std::string &str_ana, bool &b_show );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_ana_max_temp( YHANDLE h, const std::string &str_ana, unsigned short &w_x, unsigned short &w_y, float &ft_temp );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_ana_min_temp( YHANDLE h, const std::string &str_ana, unsigned short &w_x, unsigned short &w_y, float &ft_temp );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_ana_avg_temp( YHANDLE h, const std::string &str_ana, float &ft_temp );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_ana_all_temp( YHANDLE h, const std::string &str_ana, float *pft_temp, unsigned int &dw_count );

YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_set_ana_color_index( YHANDLE h, const std::string &str_ana, ColorIndex index );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_ana_color_index( YHANDLE h, const std::string &str_ana, ColorIndex &index );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_ana_has_self_color( YHANDLE h, const std::string &str_ana, bool &b_has );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_set_ana_has_self_color( YHANDLE h, const std::string &str_ana, bool b_has );

YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_set_ana_adjust_param( YHANDLE h, const std::string &str_ana, int i_min, int i_max );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_set_ana_adjust_temp( YHANDLE h, const std::string &str_ana, float ft_min, float ft_max );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_ana_adjust_param( YHANDLE h, const std::string &str_ana, int &i_min, int &i_max );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_ana_adjust_temp( YHANDLE h, const std::string &str_ana, float &ft_min, float &ft_max );

YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_set_ana_alias( YHANDLE h, const std::string &str_ana, const std::string &str_alias );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_ana_alias( YHANDLE h, const std::string &str_ana, std::string &str_alias );

YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_set_ana_des( YHANDLE h, const std::string &str_ana, const std::string &str_des );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_ana_des( YHANDLE h, const std::string &str_ana, std::string &str_des );

/**
	@brief 与额外的文本信息相关的接口

	@param str_tag 文本的标签
	@param str_text 实际的文本字符

	add_text 增加1个文本数据
	get_text 获取某个标签的文本
	delete_text 删除1个文本数据

	add/delete_text 增加或删除必须在调用save_file之后才能存储到文件中
*/
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_add_text( YHANDLE h, const std::string &str_tag, const std::string &str_text );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_get_text( YHANDLE h, const std::string &str_tag, std::string &str_text );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_delete_text( YHANDLE h, const std::string &str_tag );

#ifdef WIN32
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_has_sound( YHANDLE h, bool &b_has_sound );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_play_sound( YHANDLE h );
YFDLL_YFIRIMAGEEX1_API yf_ir_image_ex1_stop_sound( YHANDLE h );
#endif

#endif

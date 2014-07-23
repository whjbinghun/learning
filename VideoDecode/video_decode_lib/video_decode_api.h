#ifndef VIDEO_DECODE_API_H
#define VIDEO_DECODE_API_H


typedef int (*pfOutDataBuf) (unsigned char *puc_out_buf, unsigned int ui_data_len, int width, int height, int num, int den);

#ifndef DEFINE_YHANDLE
typedef void *YHANDLE;
#define DEFINE_YHANDLE
#endif

/**********************************************************************************
Func    Name: video_decode_init
Descriptions: h264转RGB的初始化函数
Input   para: p_handle：handle指针
              i_buf_len: 用于存放待解码数据的buf大小
In&Out  Para:
Output  para:
Return value: 成功返回0, 失败返回其他值
***********************************************************************************/
int video_decode_init( YHANDLE *p_handle, int i_buf_len, pfOutDataBuf pf_out_data_func );

/**********************************************************************************
Func    Name: video_decode_deinit
Descriptions: h264转RGB的释放函数
Input   para: p_handle：handle指针
In&Out  Para:
Output  para:
Return value: 成功返回0, 失败返回其他值
***********************************************************************************/
int video_decode_deinit( YHANDLE *p_handle );

/**********************************************************************************
Func    Name: video_put_data
Descriptions: 用于把需要解码的数据注入到解码池中, 在调用该函数之前建议先调用video_get_remain_buf函数
              来判断解码池是否满了
Input   para: p_handle：handle指针
              puc_buf: 存放待解码数据的buf的指针
              i_len: 存放待解码数据的buf中数据的大小
In&Out  Para:
Output  para:
Return value: 成功返回0, 失败返回其他值
***********************************************************************************/
int video_put_data( YHANDLE handle, unsigned char *puc_buf, int i_len );

/**********************************************************************************
Func    Name: video_get_remain_buf
Descriptions: 返回解码池中剩余的BUF大小, 在调用video_put_data函数之前建议先调用该函数
              来判断数据池是否有足够的BUF用于存放新的待解码数据
Input   para: p_handle：handle指针
In&Out  Para:
Output  para:
Return value: 返回解码池中剩余的BUF大小
***********************************************************************************/
unsigned int video_get_remain_buf( YHANDLE handle );

/**********************************************************************************
Func    Name: video_get_used_buf
Descriptions: 返回解码池中已存在的还未进行解码的数据
Input   para: p_handle：handle指针
In&Out  Para:
Output  para:
Return value: 返回解码池中已存在的还未进行解码的数据
***********************************************************************************/
unsigned int video_get_used_buf( YHANDLE handle );

#endif // VIDEO_DECODE_API_H

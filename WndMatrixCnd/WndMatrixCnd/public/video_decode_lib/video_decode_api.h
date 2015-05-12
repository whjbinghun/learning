#ifndef VIDEO_DECODE_API_H
#define VIDEO_DECODE_API_H

#include <sys/time.h>

/*
 * puc_out_buf: RGB����buf
 * ui_data_len: RGB���ݴ�С
 * width      : ֡���
 * height     : ֡�߶�
 * p_user_data: ����ʹ���߹���ʱ�������Ϣ, ���庬����ʹ�����Լ�����
 */
typedef int (*pfOutDataBuf) ( unsigned char *puc_out_buf, unsigned int ui_data_len, int width, int height, void *p_user_data );

#ifndef DEFINE_YHANDLE
typedef void *YHANDLE;
#define DEFINE_YHANDLE
#endif

/**********************************************************************************
Func    Name: video_decode_init
Descriptions: h264תRGB�ĳ�ʼ������
Input   para: p_handle��handleָ��
              i_buf_len: ���ڴ�Ŵ��������ݵ�buf��С
In&Out  Para:
Output  para:
Return value: �ɹ�����0, ʧ�ܷ�������ֵ
***********************************************************************************/
int video_decode_init( YHANDLE *p_handle, int i_buf_len, pfOutDataBuf pf_out_data_func, void *p_user_data=NULL );

/**********************************************************************************
Func    Name: video_decode_deinit
Descriptions: h264תRGB���ͷź���
Input   para: p_handle��handleָ��
In&Out  Para:
Output  para:
Return value: �ɹ�����0, ʧ�ܷ�������ֵ
***********************************************************************************/
int video_decode_deinit( YHANDLE *p_handle );

/**********************************************************************************
Func    Name: video_put_data
Descriptions: ���ڰ���Ҫ���������ע�뵽�������, �ڵ��øú���֮ǰ�����ȵ���video_get_remain_buf����
              ���жϽ�����Ƿ�����
Input   para: handle��handleֵ
              puc_buf: ��Ŵ��������ݵ�buf��ָ��
              i_len: ��Ŵ��������ݵ�buf�����ݵĴ�С
In&Out  Para:
Output  para:
Return value: �ɹ�����0, ʧ�ܷ�������ֵ
***********************************************************************************/
int video_put_data( YHANDLE handle, unsigned char *puc_buf, int i_len );

/**********************************************************************************
Func    Name: video_get_remain_buf
Descriptions: ���ؽ������ʣ���BUF��С, �ڵ���video_put_data����֮ǰ�����ȵ��øú���
              ���ж����ݳ��Ƿ����㹻��BUF���ڴ���µĴ���������
Input   para: handle��handleֵ
In&Out  Para:
Output  para:
Return value: ���ؽ������ʣ���BUF��С
***********************************************************************************/
unsigned int video_get_remain_buf( YHANDLE handle );

/**********************************************************************************
Func    Name: video_get_used_buf
Descriptions: ���ؽ�������Ѵ��ڵĻ�δ���н��������
Input   para: handle��handleֵ
In&Out  Para:
Output  para:
Return value: ���ؽ�������Ѵ��ڵĻ�δ���н��������
***********************************************************************************/
unsigned int video_get_used_buf( YHANDLE handle );

/**********************************************************************************
Func    Name: video_flush_decode
Descriptions: ���øú����Ὣ����⻺�����
Input   para: handle��handleֵ
In&Out  Para:
Output  para:
Return value:
***********************************************************************************/
void video_flush_decode( YHANDLE handle );

#endif // VIDEO_DECODE_API_H

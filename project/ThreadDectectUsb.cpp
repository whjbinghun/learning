#include "StdAfx.h"
#include "ThreadDectectUsb.h"

DWORD WINAPI thread_level( LPVOID lpParameter );

ThreadDectectUsb::ThreadDectectUsb(void)
{
	mn_port_status = -1;
	mn_number = 0;
	mn_level = 0;
	mb_close_thread = true;
}


ThreadDectectUsb::~ThreadDectectUsb(void)
{
}

int ThreadDectectUsb::print_device_num()
{
	// 输出连接到电脑的设备数量
	printf("共扫描到%d个设备！\r\n", mc100_scan_device());
	return mc100_scan_device();
}

bool ThreadDectectUsb::open_comm( int n_id )
{
	// 打开设备0，失败返回-1
	if ( mc100_open( n_id ) >= 0 )
	{
		printf( "打开设备%d成功！\r\n", n_id );
	}
	else
	{
		printf( "打开设备%d失败！\r\n", n_id );
		return false;
	}

	Sleep( 1000 );

	int n_thread = thread_pro( );
	if( n_thread == 0 ) {
		printf( "创建线程成功!\r\n" );
		return true;
	} else {
		printf( "创建线程失败!\r\n" );
		return false;
	}
}

bool ThreadDectectUsb::close_comm( int n_id )
{
	int n_close = mc100_close( n_id );
	if( n_close ==1 ) {
		printf( "关闭设备%d成功\r\n", n_id );
		return true;
	} else {
		printf( "关闭设备%d失败\r\n", n_id );
		return false;
	}
}


DWORD ThreadDectectUsb::thread_pro( )
{
	HANDLE handle_level;
	handle_level = CreateThread( NULL, 0, thread_level, this, 0, NULL );

	if( NULL == handle_level )
	{
		printf( "Create Thread_level failed !\r\n" );
		return -1;
	}

	CloseHandle( handle_level );
	return 0;
}

DWORD WINAPI thread_level( LPVOID lpParameter )
{
	ThreadDectectUsb *thread_dect;
	thread_dect = (ThreadDectectUsb *)lpParameter;
	while ( thread_dect->get_close_thread() ) {
		int n = mc100_check_pin( 0, MC100_PORTA );

		//当前值等于前一个值
		if( n ==  thread_dect->get_level() ) {
			int n_number = thread_dect->get_number();
			n_number++;
			thread_dect->set_number( n_number );
			thread_dect->set_level( n );
			if( n_number == 10 ) {
				thread_dect->set_status( n );
				thread_dect->mp_call_state( n );
			}	
		} else {
			thread_dect->set_level( n );
			thread_dect->set_number( 1 );
		}
		
		Sleep(100);
	}
	return 0;
}

int ThreadDectectUsb::get_number()
{
	return mn_number;
}

int ThreadDectectUsb::get_level()
{
	return mn_level;
}

void ThreadDectectUsb::set_status( int n_status )
{
	mn_port_status = n_status;
}

void ThreadDectectUsb::set_number( int n_number )
{
	mn_number = n_number;
}

void ThreadDectectUsb::set_level( int n_level )
{
	mn_level = n_level;
}

bool ThreadDectectUsb::get_close_thread()
{
	return mb_close_thread;
}

void ThreadDectectUsb::set_close_thread_status( bool b_close_thread )
{
	mb_close_thread = b_close_thread;
}

int ThreadDectectUsb::get_status()
{
	return mn_port_status;
}

void ThreadDectectUsb::set_status_number( puc_status p_status )
{
	mp_call_state = p_status;
}

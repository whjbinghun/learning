#include "stdafx.h"
#include "mc100.h"

#include <stdlib.h>
#include <windows.h>
#include "ThreadDectectUsb.h"

void mc100_test( void );
int callback_status( int n_status ){
	if( n_status==1 ) {
		printf( "ThreadDectectUsb::set_status：高电平\n" );
	} else {
		printf( "ThreadDectectUsb::set_status：低电平\n" );
	}
	return 0;
}

int main(int argc, char* argv[])
{
	mc100_test();
	Sleep(1000);
	return 0;
}

void mc100_test( void )
{
	ThreadDectectUsb thread_dectect;
	int n_device_num = thread_dectect.print_device_num();

	thread_dectect.open_comm( 0 );
	thread_dectect.set_status_number( callback_status );

	getchar();
	thread_dectect.set_close_thread_status( false );
	thread_dectect.close_comm( 0 );
	return;
}


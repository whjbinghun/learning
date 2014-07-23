#pragma once
#include "stdafx.h"
#include "mc100.h"
#include <stdlib.h>
#include <windows.h>

typedef int (*puc_status)(int);

class ThreadDectectUsb
{
public:
	ThreadDectectUsb(void);
	~ThreadDectectUsb(void);
public:

	int print_device_num();
	bool open_comm( int n_id );
	bool close_comm( int n_id );

	int get_status();
	int get_number();
	int get_level();
	void set_status( int n_status );
	void set_number( int n_number );
	void set_level( int n_level );
	DWORD thread_pro();
	bool get_close_thread( );
	void set_close_thread_status( bool b_close_thread );
	void set_status_number( puc_status p_status );
private:

public:
	puc_status mp_call_state;
private:
	
	int mn_port_status;
	int mn_number;
	int mn_level;
	bool mb_close_thread;
};


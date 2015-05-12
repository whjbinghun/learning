#ifndef NET_INTERFACE_H
#define NET_INTERFACE_H


class ITcpClientSink
{
public:
    virtual int on_connect(int n_state ) = 0;      // -1 ofline ; 1 online
    virtual int on_err( int err ) = 0;
    virtual int on_data(char *p_data,int n_len) = 0;
  // virtual int on_time_out(int timer_id) = 0;
};

class ITcpClient
{
public:
   virtual int connect(char* s_remote_ip,int n_remote_port,char* s_local_ip=0,int n_local_port=0) = 0;
   virtual int disconnect()=0;
   virtual int send(char* p_data,int n_len) =0 ;
  //virtual int schedule_timer(int n_intval) =0 ;
   virtual int set_reconnect(int n_intval=3000) =0;
    virtual int wait_connected( int n_wait = 3000 ) = 0;
};

int net_mode_start();
int net_mode_stop();

ITcpClient* create_tcpclient(ITcpClientSink *p_sink);
void destroy_tcpclient(ITcpClient* p_client);

#endif // NET_INTERFACE_H

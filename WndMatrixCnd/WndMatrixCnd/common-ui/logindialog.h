#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMutex>
#include <QWaitCondition>
#include <QMainWindow>
#include "cfgfileinfo.h"
#include "titlebar.h"
#include "logiclayer/cloginview.h"
#include "../../public/yf-net-discover/yf-net-discover.h"
#include <QComboBox>

enum eSeverInfo{
    REALTIME_SERVER = 1,
    PLAYBACK_SERVER = 2
};

typedef struct _tagSrvInfo_S{
    int i_dev_type;
    QString str_sn;
    QString str_ipv4_addr;
    QString str_ipv6_addr;
    unsigned short us_port;
    QString str_mac;
    _tagSrvInfo_S()
        : i_dev_type( 0 )
        , str_sn( "" )
        , str_ipv4_addr( "" )
        , str_ipv6_addr( "" )
        , us_port( 0 )
        , str_mac( "" ){ }
}SrvInfo_S;

class LoginDialog : public QDialog,public CLoginView
{
    Q_OBJECT
public:
    explicit LoginDialog( QMainWindow *parent = 0, bool b_realtime_status=true );
    virtual ~LoginDialog( );
    void paintEvent( QPaintEvent *event );

public:
    virtual void closeEvent( QCloseEvent* event );
    virtual int onResponse(CBaseMsg& reqMsg,CBaseMsg& rspMsg);
    virtual int onNotify(CBaseMsg& msg);

    void create_ctrl();
    void init_ctrl();
    void init_ctrl_font();
    void init_auto_find_server();

    bool detection_conn_is_correct( QString str_ip, QString str_port );
    bool detection_login_is_correct( QString str_user, QString str_pwd );
    bool send_link_auth( QString str_user, QString str_pwd );
    bool send_ver_nego();

    void set_ctrl_enable( bool b_enable );

    //int on_parse_auth(  const MsgHeader &package_head, unsigned char *p_buf, unsigned int n_len_buf  );
    //int on_parse_ver(  const MsgHeader &package_head, unsigned char *p_buf, unsigned int n_len_buf );
    void keyPressEvent(QKeyEvent *event);

    void write_to_cfg();
    void move_login();
    void show_login();
    void show_warn_info(QString str_warning);
    void show_ip_and_port();
    //获取登陆信息
    void get_client_login_info( QString &str_user, QString &str_password, QString &str_ip, QString &str_port );
    QString get_user();    //返回用户名
    QString get_password(); //返回密码
    QString get_ip();       //返回ip
    QString get_port();    //返回端口号
signals:
    void sig_close_login_full();
    void sig_add_server( QString );
public slots:
    void login_clicked();
    void slot_close_login();
    void slot_change();
    void slot_exit(){
        close();
    }

    void slot_timer_find_server();
    void slot_add_server( QString str_sn );
private:
    QPushButton *mp_btn_login;
    QPushButton *mp_btn_cancel;
    //QPushButton *mp_btn_change;

    //QLabel *mp_lab_ip;
    //QLabel *mp_lab_port;
    QLabel *mp_lab_name;
    QLabel *mp_lab_pwd;
    QLabel *mp_lab_warninfo;
    QLabel *mp_lab_server_sn;

    //QLineEdit *mp_edit_ip;
    //QLineEdit *mp_edit_port;
    QLineEdit *mp_edit_name;
    QLineEdit *mp_edit_pwd;
    QComboBox *mp_box_server_sn;

    QString m_str_cfg_path;

    QFrame *mp_login_frame;

    TitleBar *mp_titlebar;
    QPixmap m_pixmap_backgroup;

    QMutex m_mutex_auth;
    QWaitCondition m_mutex_condition_auth;

    bool mb_realtime_status;//0表示实时，1表示回放

    //2015.3.12 blm add  保存客户端登陆信息
    QString m_str_user;  //用户名
    QString m_str_password; //密码
    QString m_str_ip;   //ip
    QString m_str_port; //端口号

    QTimer *mp_timer_find_server;
};

#endif // LOGINDIALOG_H

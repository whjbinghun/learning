#ifndef CBASEMDL_H
#define CBASEMDL_H
#include "cbasemsg.h"
#include <QThread>
#include <QMutex>
#include <QTime>
#include <list>
#include <map>

typedef map<int,CBaseMsg> MapMsg;
typedef list<CBaseMsg>    MsgList;

using namespace std;

enum MdlType
{
  UnknowMdl,
  ControlMdl,
  IrMonitorMdl,
  ViMonitorMdl,
  AlarmHandleMdl,
  AllMdl,
  UiAppMdl,
  PlayBackMdl,
};

class CBaseMdl : public QThread
{
public:
    CBaseMdl(CBaseMdl * pOutPut);
    virtual ~CBaseMdl();

    virtual int postMsg(CBaseMsg& msg);
    virtual int start(int n_stacksize,int n_priority);
    virtual int stop();
    virtual int  processMsg(); // return -1 do nothing ,other ddo some thing
protected:
    virtual void run() ;                       
    int         pushMsg(CBaseMsg  & msg);               //暂存请求消息
    bool        popMsg(int nSeq,CBaseMsg & msg);      //获取前面暂存的消息
    virtual int handleRequest(CBaseMsg &msgmsgReq);
    virtual int handleNotify(CBaseMsg &msg);
    virtual int handleResponse(CBaseMsg &msgRsp);
    void        checkTimeOutReq();
    void        handleTimeOutReq(int id,CBaseMsg &msgReq);
    virtual int preHandleMsgTimeout(int id,CBaseMsg &msgReq);
    void        clearMsg();

protected:
    QTime     m_lastCheck;
    QMutex    m_mtxList;
    MsgList   m_msglist;
    QMutex    m_mtxMap;
    MapMsg    m_mapMsg;
    CBaseMdl* m_pOutput;
    int m_nMsgSeqGenerator;
    volatile bool m_bQuit;

};

#endif // CBASEMDL_H

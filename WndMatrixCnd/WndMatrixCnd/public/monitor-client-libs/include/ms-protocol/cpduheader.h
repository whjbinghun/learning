#ifndef CPDUHEADER_H
#define CPDUHEADER_H
#include "iprotocol.h"
#include "netcommon.h"


class CPduHeader : public IPdu
{
public:
    CPduHeader();
    virtual ~CPduHeader();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    MsgHeader m_msghdr;
};

typedef CPduHeader CPduHeartbeatAck;

class CBaseAck : public CPduHeader
{
public:
    CBaseAck();
    virtual ~CBaseAck();

    virtual int type();
    virtual int length();
    virtual int encode(unsigned char* pbuff, int nlen);
    virtual int decode(unsigned char* pbuff, int nlen);
    virtual void dump();

public:
    int    m_result;
};

#endif // CPDUHEADER_H

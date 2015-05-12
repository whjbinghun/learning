#ifndef IPROTOCOL_H
#define IPROTOCOL_H

class IPdu
{
public:
    IPdu() {}
    virtual ~IPdu() {}

    virtual int type()=0;
    virtual int length()=0;
    virtual int encode(unsigned char* pbuff, int nlen)=0;
    virtual int decode(unsigned char* pbuff, int nlen)=0;
    virtual void dump()=0;
};


class IParser
{
public:
    IParser() {}
    virtual ~IParser() {}
    virtual int parse(char* pbuff,int nlen)=0;
    virtual void reset()=0;
};

class IParserSink
{
public:

    virtual int onPdu(IPdu* ppdu)=0;
    virtual void onError(int nerr)=0;
};

IParser* CreateParser(IParserSink * psink);
void     DestroyParser(IParser* pparser);

#endif // IPROTOCOL_H

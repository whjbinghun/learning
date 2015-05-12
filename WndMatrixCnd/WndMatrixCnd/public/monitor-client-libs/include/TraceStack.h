#ifndef _H_MS_TRACE_STACK
#define _H_MS_TRACE_STACK

class CTraceStack
{
public:
    CTraceStack(int nId,int nLevel,char* szFile,char* szTime,char* szFunction,int nLine);
    virtual ~CTraceStack(void);
	void SetId(int nId);
	void SetIp(char* szIp);
	void SetResult(int nResult);
protected:
	int       m_nId;
    int       m_nLevel;
    char*     m_szFile;
    char*     m_szTime;
    char*     m_szFunction;
    int       m_nLine;
	char      m_szIp[32];
	int		  m_nResult;

};

#endif

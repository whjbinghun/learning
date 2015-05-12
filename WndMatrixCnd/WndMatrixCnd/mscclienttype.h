#ifndef MSCLIENTTYPE_H
#define MSCLIENTTYPE_H

#include "cbasemsg.h"
/*
 *MSCClientType类， 对各个客户的类型进行管理
 *
 *
 *
 *
 * */

class MSCClientType
{
public:
    MSCClientType();
    ~MSCClientType();

    static MSCClientType* Instance();
    static void Release();

    void set_client_type( ClientType client_type);
    ClientType get_client_type();
private:
    static MSCClientType* m_client_type_class;

    ClientType me_client_type;
};

#endif // MSCLIENTTYPE_H

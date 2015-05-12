#include "mscclienttype.h"

MSCClientType* MSCClientType::m_client_type_class = 0;

MSCClientType::MSCClientType()
{
    me_client_type = ClientNone;
}

MSCClientType::~MSCClientType()
{

}

MSCClientType *MSCClientType::Instance()
{
    if (!m_client_type_class){
        m_client_type_class = new MSCClientType();
    }
    return m_client_type_class;
}

void MSCClientType::Release()
{
    if (m_client_type_class){
        delete m_client_type_class;
        m_client_type_class = 0;
    }
}

void MSCClientType::set_client_type(ClientType client_type)
{
    me_client_type = client_type;
}

ClientType MSCClientType::get_client_type()
{
    return me_client_type;
}

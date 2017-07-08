#ifndef SOCKETDATA_H
#define SOCKETDATA_H

#include <QString>

#include "networkserver_global.h"

class QTcpSocket;

#define _INCLUDEFUNTION(name, type)\
    private:\
        type _##name;

#define _SETFUNTION(name, type) \
    public:\
        void set_##name(type name){_##name = name;}

#define _GETFUNTION(name, type) \
    public:\
        type name() const{return _##name;}

#define SETONLYFUNTION(name, type)\
    _SETFUNTION(name, type)\
    _INCLUDEFUNTION(name, type)

#define GETONLYFUNTION(name, type)\
    _GETFUNTION(name, type)\
    _INCLUDEFUNTION(name, type)

#define SETGETFUNTION(name, type)\
    _SETFUNTION(name, type)\
    _GETFUNTION(name, type)\
    _INCLUDEFUNTION(name, type)


class NETWORKSERVERSHARED_EXPORT SocketData
{
public:
    SocketData();

    SETGETFUNTION(socket, QTcpSocket *)
    SETGETFUNTION(username, QString)
};

#endif // SOCKETDATA_H

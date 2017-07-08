#include "networkserver.h"

#include <QDebug>
#include <QTcpSocket>

#include "socketdata.h"

NetworkServer *NetworkServer::initialization()
{
    static NetworkServer networkServer;
    return &networkServer;
}

void NetworkServer::listen(quint16 port)
{
    server->listen(QHostAddress::AnyIPv4, port);
}

NetworkServer::NetworkServer() :
    server(new QTcpServer)
{
    connect(server, &QTcpServer::newConnection, this, [=]{
        QTcpSocket *socket = server->nextPendingConnection();

        emit newConnected(socket);
    });
}

NetworkServer::~NetworkServer()
{

}

NetworkServer::NetworkServer(const NetworkServer &)
{

}

NetworkServer &NetworkServer::operator=(const NetworkServer &)
{
    return *this;
}

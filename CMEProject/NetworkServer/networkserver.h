#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <QObject>

#include "networkserver_global.h"

#include <QTcpServer>

class SocketData;

class NETWORKSERVERSHARED_EXPORT NetworkServer : public QObject
{
    Q_OBJECT
public:
    static NetworkServer* initialization();
    void listen(quint16 port);

signals:
    void newConnected(QTcpSocket *);

public slots:

private:
    NetworkServer();
    ~NetworkServer();
    NetworkServer(const NetworkServer &);
    NetworkServer& operator=(const NetworkServer &);

    QTcpServer *server;
};

#endif // NETWORKSERVER_H

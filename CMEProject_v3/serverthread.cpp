#include "serverthread.h"

#include <QHostAddress>
#include "defineheader.h"

#include "server.h"

ServerThread::ServerThread(QObject *parent) :
    QThread(parent)
{

}

ServerThread::~ServerThread()
{
    qDebug() << "ServerThread::~ServerThread()";
}

ServerThread &ServerThread::initialization()
{
    static ServerThread serverThread;
    return serverThread;
}

void ServerThread::run()
{
    //qDebug() << "ServerThread::run()";
    server = &Server::initialization();
    server->listen(QHostAddress::Any, 55512);

    exec();
}

#include "server.h"

#include <QDebug>
#include <QThread>

#include "controlthread.h"

Server &Server::initialization()
{
    static Server server;
    return server;
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << QThread::currentThreadId() << "Server::incomingConnection";
    ControlThread *thread = new ControlThread(socketDescriptor);
    connect(thread, &ControlThread::finished, this, [=]{threadFinishing(thread);});
    connect(thread, &ControlThread::sendToAllMessage, this, [=](const QString &msg){
        for(ControlThread *otherThread : threads){
            if(otherThread != thread)
                otherThread->sendMessage(msg);
        }
    });

    threads.push_back(thread);
    thread->start();

    connect(thread, &ControlThread::enterEventLoop, this, [=]{
        emit newControlThread(thread);
    });
}

Server::Server(QObject *parent) : QTcpServer(parent)
{

}

Server::~Server()
{
    qDebug() << "Server::~Server()";
}

void Server::threadFinishing(ControlThread *thread)
{
    qDebug() << QThread::currentThreadId() << "Server::incomingConnection ControlThread::finished";
    threads.removeOne(thread);

    emit threadFinished();
}

#include "networkserver.h"

#include "Control/controlthread.h"

NetworkServer::NetworkServer(QObject *parent) : QTcpServer(parent)
{
    // ************** test ********************
    //incomingConnection(0);
}

QList<ControlThread *>& NetworkServer::getControlThreads()
{
    return controlThreads;
}

void NetworkServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << QThread::currentThreadId() << "newConnected";

    ControlThread *thread = new ControlThread(socketDescriptor);
    emit newConnected(thread);

    connect(thread, &ControlThread::finished, this, [=]{
        controlThreads.removeOne(thread);
    });
    controlThreads.push_back(thread);
    thread->start();
}

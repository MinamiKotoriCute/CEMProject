#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <QObject>
#include <QTcpServer>

class ControlThread;

class NetworkServer : public QTcpServer
{
    Q_OBJECT
public:
    NetworkServer(QObject *parent = 0);
    QList<ControlThread *> &getControlThreads();

signals:
    void newConnected(ControlThread*);

public slots:

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    QList<ControlThread *> controlThreads;
};

#endif // NETWORKSERVER_H

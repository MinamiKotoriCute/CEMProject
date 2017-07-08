#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

class ControlThread;

class Server : public QTcpServer
{
    Q_OBJECT
public:
    static Server& initialization();

signals:
    void threadFinished();
    void newControlThread(ControlThread *thread);

public slots:

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    explicit Server(QObject *parent = 0);
    Server(const Server &);
    Server operator=(Server const&);
    ~Server();

    void threadFinishing(ControlThread *thread);

    QList<ControlThread *> threads;
};

#endif // SERVER_H

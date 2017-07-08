#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <QHostAddress>

#include "server.h"

class ServerThread : public QThread
{
    Q_OBJECT
public:
    static ServerThread &initialization();

signals:

public slots:

protected:
    void run();

private:
    explicit ServerThread(QObject *parent = 0);
    ServerThread(const ServerThread &);
    ServerThread operator=(ServerThread const&);
    ~ServerThread();

    Server *server;
};

#endif // SERVERTHREAD_H

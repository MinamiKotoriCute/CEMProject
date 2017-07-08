#ifndef CONTROLTHREAD_H
#define CONTROLTHREAD_H

#include <QThread>

class QTcpSocket;
class Control;

class ControlThread : public QThread
{
    Q_OBJECT
public:
    explicit ControlThread(qintptr socketDescriptor, QObject *parent = 0);
    QJsonObject* getJsonDataPtr();

signals:
    void sendMessage(const QString &);
    void sendToAllMessage(const QString &);
    void enterEventLoop();

public slots:

protected:
    void run();

private:
    QTextCodec *codec;
    QTcpSocket *socket;
    Control *control;
    qintptr socketDescriptor;
};

#endif // CONTROLTHREAD_H

#ifndef CONTROLTHREAD_H
#define CONTROLTHREAD_H

#include <QObject>
#include <QThread>
#include <QDateTime>

class QTcpSocket;
class Control;

class ControlThread : public QThread
{
    Q_OBJECT
public:
    explicit ControlThread(qintptr socketDescriptor, QObject *parent = 0);

    Control *getContorl();
    Qt::HANDLE getThreadId();
    QDateTime getCreateDateTime();

signals:
    void disconnected();
    void print(QString);

public slots:

protected:
    void run();

private:
    qintptr socketDescriptor;
    QTcpSocket *socket;
    Control *control;
    Qt::HANDLE threadId;
    QDateTime createDateTime;
};

#endif // CONTROLTHREAD_H
